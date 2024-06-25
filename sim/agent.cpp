#include <iostream>
#include <random>
#include <tuple>

#define K 16
#define T1 6
#define T2 4
#define T3 2
#define T_DASH 20
#define OVEREST 1
#define MAX_SIZE (60*OVEREST)
#define MAX_TIME_SIZE (25*MAX_SIZE)

class Agent {
public:
    uint_fast16_t time;
    uint_fast8_t max;
    uint_fast8_t lastMax;
    uint_fast16_t interactions;

    Agent() {
        time = T3;
        max = 1;
        lastMax = 1;
        interactions = T3;
    }

    Agent(std::uniform_int_distribution<> &time_dist, std::uniform_int_distribution<> &max_dist,
          std::uniform_int_distribution<> &interactions_dist, std::ranlux48_base &gen) {
        time = time_dist(gen);
        max = max_dist(gen);
        lastMax = max_dist(gen);
        interactions = interactions_dist(gen);
    }

    bool operator==(const Agent &other) const {
        return time == other.time && max == other.max && lastMax == other.lastMax && interactions == other.interactions;
    }
};


std::ostream &operator<<(std::ostream &os, const Agent &agent) {
    os << "timer: " << agent.time << ", max: " << agent.max << ", estimate: " << agent.lastMax << ", interactions: "
       << agent.interactions << std::endl;
    return os;
}
