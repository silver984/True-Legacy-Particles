#include <tlp/Settings.hpp>
#include <Geode/loader/Mod.hpp>

namespace tlp
{

void Settings::update()
{
	m_isModEnabled = geode::Mod::get()->getSettingValue<bool>("switch");
}

} // tlp