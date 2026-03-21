#include <tlp/Settings.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Log.hpp>

#define SETTINGS_LISTENER(TYPE, KEY, VAR)   \
    geode::listenForSettingChanges<TYPE>(   \
    KEY,                                    \
    [](TYPE val)                            \
        {                                   \
            VAR = val;                      \
        }                                   \
    )

$execute
{
    SETTINGS_LISTENER(bool, "switch", tlp::g_isModEnabled);
    SETTINGS_LISTENER(bool, "legacy-tracking", tlp::g_isLegacyTracking);
    SETTINGS_LISTENER(bool, "legacy-scaling", tlp::g_isLegacyScaling);
    SETTINGS_LISTENER(bool, "legacy-values", tlp::g_isLegacyValues);
    SETTINGS_LISTENER(bool, "legacy-rotation", tlp::g_isLegacyRotation);
    SETTINGS_LISTENER(bool, "player-1", tlp::g_worksForP1);
    SETTINGS_LISTENER(bool, "player-2", tlp::g_worksForP2);
}