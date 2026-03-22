#include <tlp/CCParticleSystem.hpp>
#include <tlp/Settings.hpp>
#include <Geode/loader/Log.hpp>

void TLPCCParticleSystem::loadScaledDefaults(float v)
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCParticleSystem::loadScaledDefaults(v);
		return;
	}

	if (std::string(getID()).starts_with(geode::Mod::get()->getID()))
	{
		return;
	}
	
	cocos2d::CCParticleSystem::loadScaledDefaults(v);
}

void TLPCCParticleSystem::resetSystem()
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCParticleSystem::resetSystem();
		return;
	}

	if (getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr)
	{
		return;
	}

	cocos2d::CCParticleSystem::resetSystem();
}

void TLPCCParticleSystem::setAngle(float var)
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCParticleSystem::setAngle(var);
		return;
	}

	if (getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr || getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr)
	{
		return;
	}
	
	cocos2d::CCParticleSystem::setAngle(var);
}

void TLPCCParticleSystem::setAngleVar(float var)
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCParticleSystem::setAngleVar(var);
		return;
	}

	if (getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr || getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr)
	{
		return;
	}
	
	cocos2d::CCParticleSystem::setAngleVar(var);
}

void TLPCCParticleSystem::setGravity(const cocos2d::CCPoint& g)
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCParticleSystem::setGravity(g);
		return;
	}

	if (getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr || getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr)
	{
		return;
	}
	
	cocos2d::CCParticleSystem::setGravity(g);
}

void TLPCCParticleSystem::setPosVar(cocos2d::CCPoint const& var)
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCParticleSystem::setPosVar(var);
		return;
	}

	if (getID() == "player-ground-particles"_spr || getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr)
	{
		return;
	}
	
	cocos2d::CCParticleSystem::setPosVar(var);
}

void TLPCCParticleSystem::setSpeed(float speed)
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCParticleSystem::setSpeed(speed);
		return;
	}

	if (getID() == "player-ground-particles"_spr || getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr)
	{
		return;
	}
	
	cocos2d::CCParticleSystem::setSpeed(speed);
}

void TLPCCParticleSystem::setSpeedVar(float speed)
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCParticleSystem::setSpeedVar(speed);
		return;
	}

	if (getID() == "player-ground-particles"_spr || getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr)
	{
		return;
	}

	cocos2d::CCParticleSystem::setSpeedVar(speed);
}
