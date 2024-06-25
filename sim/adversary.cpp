#include <algorithm>
#include <vector>
#include "agent.cpp"
#include <fstream>

using namespace std;


class Logger {
private:
    std::uint_fast32_t timers[MAX_TIME_SIZE] = {0}, interactions[MAX_TIME_SIZE + 1] = {0};
    std::uint_fast32_t max[MAX_SIZE + 1] = {0}, estimate[MAX_SIZE + 1] = {0};
    std::uint_fast32_t resamples = 0, n = 0;
    std::string filepath;

public:
    explicit Logger(std::string filepath) : filepath(std::move(filepath)) {}

    Logger() : Logger("outputs.csv") {}

    void reset() {
        std::fill(timers, timers + MAX_TIME_SIZE, 0);
        std::fill(max, max + MAX_SIZE + 1, 0);
        std::fill(estimate, estimate + MAX_SIZE + 1, 0);
        std::fill(interactions, interactions + MAX_TIME_SIZE + 1, 0);
        n = 0;
    }

    void log_agent(Agent a) {
        if (a.time <= MAX_TIME_SIZE) ++timers[a.time];
        else cout << "--------------------------- time " << a.time << endl;
        if (a.max <= MAX_SIZE) ++max[a.max];
        else cout << "--------------------------- max " << a.max << endl;
        if (a.lastMax <= MAX_SIZE) ++estimate[a.lastMax];
        else cout << "--------------------------- lastMax " << a.lastMax << endl;
        if (a.interactions <= MAX_TIME_SIZE) ++interactions[a.interactions];
        else cout << "--------------------------- interactions " << a.interactions << " " << MAX_TIME_SIZE << endl;
        ++n;
    }

    void log_agent_remove(Agent a) {
        --timers[a.time];
        --max[a.max];
        --estimate[a.lastMax];
        --interactions[a.interactions];
        --n;
    }

    void log_resample() {
        ++resamples;
    }

    void setup_logger() {
        std::ofstream file(filepath);
        file << "time;timer;max;estimate;interactions;resamples;n\n";
        file.close();
        save_logger(0);

    }

    void save_logger(unsigned int iteration) {
        std::ofstream file(filepath, std::ios_base::app);
        file << iteration << ";"; //<<  << "," std::endl;
        for (const unsigned int &num: timers) {
            file << num << ",";
        }
        file << ";";
        for (const unsigned int &num: max) {
            file << num << ",";
        }
        file << ";";
        for (const unsigned int &num: estimate) {
            file << num << ",";
        }
        file << ";";
        for (const unsigned int &num: interactions) {
            file << num << ",";
        }
        file << ";" << resamples << ";" << n << "\n";
        file.close();
        resamples = 0;
    }
};


class Adversary {
public:
    Adversary() {
        times = std::vector<int>();
        targets = std::vector<unsigned long>();
    }

    void change(int time, unsigned long target) {
        times.push_back(time);
        targets.push_back(target);
    }

    void enact(unsigned int time, std::vector<Agent> &population, Logger &logger) {
        while (index < times.size() && time >= times[index]) {
            // increase population
            if (targets[index] > population.size()) {
                cout << "increasing " << endl;
                int start = (int) population.size();
                population.resize(targets[index]);
                for (int i = start; i < (int) population.size(); i++) {
                    Agent a;
                    population[i] = a;
                    logger.log_agent(a);
                }
                // decrease population
            } else if (targets[index] < population.size()) {
                cout << "decreasing " << endl;
                for (int i = (int) targets[index]; i < (int) population.size(); i++) {
                    logger.log_agent_remove(population[i]);
                }
                population.resize(targets[index]);
            }
            index++;
        }
    }


private:
    int index = 0;
    std::vector<int> times;
    std::vector<unsigned long> targets;
};

