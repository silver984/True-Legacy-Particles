#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/loader/SettingV3.hpp>

$on_mod(Loaded)
{
	auto toggleHooks =
		[](bool val) -> void
		{
			for (auto& hook : geode::Mod::get()->getHooks())
			{
				(void)hook->toggle(val);
			}
		};

	// we check for the value on mod load
	// "why do we do this if the listener is already coded below?"
	// `geode::listenForSettingChanges<>` doesn't seem to catch the setting value on mod load
	toggleHooks(geode::Mod::get()->getSettingValue<bool>("switch"));
	(void)geode::listenForSettingChanges<bool>("switch", toggleHooks);
}