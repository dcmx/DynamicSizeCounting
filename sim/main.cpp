#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <utility>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include "adversary.cpp"

#define OUTPUT_RATE_LIMIT 500

class Simulator {
public:
    std::vector<Agent> agents;
    Adversary* adversary;

    std::random_device rd;
    std::ranlux48_base gen;
    std::uniform_int_distribution<int> coin;
    Logger logger;

    Simulator(unsigned long n, int random_max, Adversary* adversary_config, const string& log_path) : rd(), gen(rd()), coin(0, 1) {
        logger = Logger(log_path);
        adversary = adversary_config;
        agents.resize(n);

        if (random_max > 1) {
            std::uniform_int_distribution<> time_dist(0, T3 * random_max);
            std::uniform_int_distribution<> max_dist(1, random_max);
            std::uniform_int_distribution<> interactions_dist(0, T3 * random_max);
            for (auto &agent: agents) {
                Agent a(time_dist, max_dist, interactions_dist, gen);
                agent = a;
                logger.log_agent(a);
            }
        } else {
            for (auto &agent: agents) {
                Agent a;
                agent = a;
                logger.log_agent(a);
            }
        }
    }

    int geometric() {
        int max = 0;
        for (int i = 0; i < K; ++i) {
            logger.log_resample();
            int x = 1;
            while (coin(gen) == 0) {
                ++x;
            }
            if (x > max) max = x;
        }
        return max;
    }

    void transition(Agent& u, Agent& v) {
        if (u.time <= 0 || (u.time <= T3 * u.max && v.time > T2 * v.max) || (u.time < T2 * u.max && u.max != v.max)) {
            u.interactions = 0;
            u.lastMax = u.max;
            u.max = geometric() * OVEREST;
            u.time = T1 * (u.lastMax < u.max ? u.max : u.lastMax);
            logger.log_resample();
        }
        if (u.interactions > T_DASH * (u.max < u.lastMax ? u.lastMax : u.max)) {
            u.interactions = 0;
            int grv = geometric();
            logger.log_resample();
            if (grv > u.max) {
                u.max = grv * OVEREST;
                u.time = T1 * u.max;
            }
        }
        if (u.time > T2 * u.max && v.time > T2 * v.max && u.max < v.max) {
            u.time = T3 * v.max;
            u.max = v.max;
        }
        if (u.max == v.max && !(u.time > T2 * u.max && v.time < T3 * v.max)) {
            u.lastMax = u.lastMax < v.lastMax ? v.lastMax : u.lastMax;
        }
        u.time = (u.time < v.time ? v.time : u.time) - 1;
        u.interactions++;

    }

    void simulate(unsigned int iterations, unsigned int resolution) {
        logger.setup_logger();
        std::atomic_uint_fast16_t outputLimitedResolution = OUTPUT_RATE_LIMIT / resolution;
        for (std::uint_fast32_t i = 1; i < iterations / resolution + 1; i++) {
            adversary->enact(i * resolution, agents, logger);
            std::uniform_int_distribution<int> dis(0, (int) agents.size() - 1);
            for (std::uint_fast32_t j = 0; j < resolution * agents.size(); j++) {
                int a = dis(gen);
                int b = dis(gen);
                while (a == b) {
                    b = dis(gen);
                }
                transition(agents[a], agents[b]);
            }
            logger.reset();
            for (const Agent &a : agents) {
                logger.log_agent(a);
            }
            if (i % outputLimitedResolution == 0) {
                std::cout << "iteration " << i * resolution << std::endl;
            }
            logger.save_logger(i * resolution);
        }
    }
};

int main(int argc, char* argv[]) {
    int exp=5, exp_end=5;
    int processes = 8, sub_jobs = 1, iterations=5000, resolution=1;
    int offset=0;
    int random_max = 10;
    bool adversarial = true;
    string dir = "../outputs";
    
    // Check for command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--exp") {
            i++;
            if (i < argc) {
                exp = std::stoi(argv[i]);
            }
        } else if (arg == "--exp_end") {
            i++;
            if (i < argc) {
                exp_end = std::stoi(argv[i]);
            }
        } else if (arg == "--random") {
            i++;
            if (i< argc) {
                random_max = std::stoi(argv[i]);
            }
        } else if (arg == "--dir") {
            i++;
            if (i < argc) {
                dir = argv[i];
            }
        } else if (arg == "--adversary") {
            i++;
            if (i < argc) {
                istringstream(argv[i]) >> std::boolalpha >> adversarial;
            }
        } else {
            std::cerr << "Unknown command-line argument: " << arg << std::endl;
            return 1;
        }
    }
    mkdir(dir.c_str(), 0777);

    for (; exp < exp_end + 1; exp++) {
        unsigned int n = (int) std::pow(10, exp);

        pid_t pid;
        for (unsigned int i = 0; i < processes; i++)
        {
            pid = fork();
            if (pid == 0)
            {
                for (unsigned int j = offset; j < sub_jobs; j++) {
                    std::cout << "-------- process: " << i << ", loop: " << j << ", n: " << n << ", max_rand: " << random_max << ", adversary: " << adversarial << " --------" << std::endl;
                    Adversary adversary = Adversary();
                    if (adversarial) {
                        adversary.change(1350, 500);
                    }
                    string name = dir + "/random_max=" + to_string(random_max) + "_n=" + to_string(n) + "_" + to_string(i + processes * j);
                    if (adversarial) {
                        name += "_adversary";
                    }
                    Simulator s = Simulator(n, random_max, &adversary, name + ".csv");

                    s.simulate(iterations, resolution);
                }
                exit(0);
            }
        }
        for (int i = 0; i < 8; i++)
            wait(nullptr);
    }
    return 0;

}
