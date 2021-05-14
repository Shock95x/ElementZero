#include <yaml.h>

inline struct Settings {

    int seed = 0;

    template <typename IO> static inline bool io(IO f, Settings &settings, YAML::Node &node) {
        return f(settings.seed, node["seed"]);
    }
} settings;