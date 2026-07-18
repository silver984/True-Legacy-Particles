#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Settings.hpp>

namespace {
static bool s_is2p1ScalingEnabled = false;
template <typename Callback>
void addListener(std::string_view key, Callback&& cb) {
    // add the listener using geode's api but also call the callback before
    // doing so. i just think its much more intuitive
    cb(geode::Mod::get()->getSettingValue<bool>(key));
    geode::listenForSettingChanges<bool>(std::string(key), std::move(cb));
};
} // namespace

namespace settings {
bool is2p1ScalingEnabled() { return s_is2p1ScalingEnabled; }
} // namespace settings

$on_mod(Loaded) {
    addListener("toggle", [](bool val) -> void {
        for (auto& hook : geode::Mod::get()->getHooks())
            (void) hook->toggle(val);
    });
    addListener("2.1-scaling",
                [](bool val) -> void { s_is2p1ScalingEnabled = val; });
}