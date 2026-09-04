#include <Geode/Geode.hpp>

#include <tlp/Settings.hpp>

#include <functional>

using namespace geode::prelude;

namespace {

static bool s_is2p1ScalingEnabled = false;

template <class Callback, class T>
concept settings_callback =
    std::invocable<Callback, T> && std::same_as<std::invoke_result_t<Callback, T>, void>;

template <class T, settings_callback<T> Callback>
void settingsListener(std::string_view key, Callback&& callback) {
    std::invoke(callback, Mod::get()->getSettingValue<T>(key));
    listenForSettingChanges<T>(std::string(key), std::move(callback));
};

} // namespace

namespace tlp::settings {

bool is2p1ScalingEnabled() {
    return s_is2p1ScalingEnabled;
}

} // namespace tlp::settings

$on_mod(Loaded) {
    settingsListener<bool>("toggle", [](bool val) -> void {
        for (auto& hook : geode::Mod::get()->getHooks()) {
            (void)hook->toggle(val);
        }
    });

    settingsListener<bool>("2.1-scaling", [](bool val) -> void { s_is2p1ScalingEnabled = val; });
}