#include <tlp/Settings.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
    tlp::settings::listener<bool>("toggle", [](bool val) -> void {
        for (auto& hook : Mod::get()->getHooks()) {
            (void)hook->toggle(val);
        }
    });
}