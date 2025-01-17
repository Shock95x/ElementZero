#include <functional>
#include <Core/core.h>
#include <Level/LevelData.h>
#include <hook.h>
#include <base/log.h>

#include "global.h"

DEF_LOGGER("Experimental");

THook(bool, "?isBaseCodeBuilderEnabled@EducationOptions@@SA_NXZ") {
    if (settings.education_feature) return true;
    return original();
}

THook(bool, "?isChemistryEnabled@EducationOptions@@SA_NXZ") {
    if (settings.education_feature) return true;
    return original();
}

THook(bool, "?isCodeBuilderEnabled@EducationOptions@@SA_NXZ") {
    if (settings.education_feature) return true;
    return original();
}

TClasslessInstanceHook(bool, "?isEducationEditionLevel@LevelData@@QEBA_NXZ") {
    if(settings.education_feature) {
        return true;
    }
    return original(this);
}

THook(bool, "?isEduMode@ConnectionRequest@@QEBA_NXZ", void *self) {
    if (settings.education_feature) return true;
    return original(self);
}

TClasslessInstanceHook(bool, "?DataDrivenBiomes@Experiments@@QEBA_NXZ") {
    if(settings.force_experiments) {
        return settings.custom_biomes;
    }
    return original(this);
}

TClasslessInstanceHook(bool, "?DataDrivenItems@Experiments@@QEBA_NXZ") {
    if(settings.force_experiments) {
        return settings.holiday_creator_features;
    }
    return original(this);
}

TClasslessInstanceHook(bool, "?Scripting@Experiments@@QEBA_NXZ") {
    if(settings.force_experiments) {
        return settings.additional_modding;
    }
    return original(this);
}

TClasslessInstanceHook(bool, "?Gametest@Experiments@@QEBA_NXZ") {
    if(settings.force_experiments) {
        return settings.gametest_framework;
    }
    return original(this);
}

TClasslessInstanceHook(bool, "?isEnabled@FeatureToggles@@QEBA_NW4FeatureOptionID@@@Z", int id) {
    if (settings.force_experiments) return true;
    return original(this, id);
}

TClasslessInstanceHook(
    LevelData,
    "?getLevelData@ExternalFileLevelStorageSource@@UEBA?AVLevelData@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
    std::string const &level_name) {
  auto data                                = original(this, level_name);
  data.mEducationFeaturesEnabled           = settings.education_feature;
  data.mGameRules.rules[18].value.val_bool = true;
  return data;
}

TClasslessInstanceHook(
    void, "?forEachIn@ResourcePack@@QEBAXAEBVPath@Core@@V?$function@$$A6AXAEBVPath@Core@@@Z@std@@H_N@Z",
    Core::Path const &path, std::function<void(Core::Path const &)> fn, int a, bool flag) {
  if (settings.debug_packs) {
    DEF_LOGGER("PACKDEBUG");
    LOGV("load packs: %p %p %d %d") % (void *) this % path.data % a % flag;
    original(
        this, path,
        [=](auto const &path) {
          LOGV("\t%s") % path.data;
          fn(path);
        },
        a, flag);
  } else {
    original(this, path, fn, a, flag);
  }
}