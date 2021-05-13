#include <yaml.h>

inline struct Settings {

    bool force_experiments           = false;
    bool holiday_creator_features    = false;
    bool custom_biomes               = false;
    bool additional_modding          = false;
    bool gametest_framework          = false;

    bool education_feature           = false;
    bool debug_packs                 = false;

    template <typename IO> static inline bool io(IO f, Settings &settings, YAML::Node &node) {
        return f(settings.force_experiments, node["force-experiments"]) &&
               f(settings.holiday_creator_features, node["holiday-creator-features"]) &&
               f(settings.custom_biomes, node["custom-biomes"]) &&
               f(settings.additional_modding, node["additional-modding"]) &&
               f(settings.gametest_framework, node["gametest-framework"]) &&
               f(settings.education_feature, node["education-feature"]) &&
               f(settings.debug_packs, node["debug-packs"]);
    }
} settings;