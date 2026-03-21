#include <tlp/CCParticleSystem.hpp>
#include <tlp/Settings.hpp>

void TLPCCParticleSystem::loadScaledDefaults(float v)
{
	if (!tlp::g_isModEnabled || !tlp::g_isLegacyScaling)
	{
		cocos2d::CCParticleSystem::loadScaledDefaults(v);
		return;
	}

	if (!std::string(this->getID()).starts_with(geode::Mod::get()->getID()))
	{
		cocos2d::CCParticleSystem::loadScaledDefaults(v);
	}
}

void TLPCCParticleSystem::setAngle(float var)
{
	if (!tlp::g_isModEnabled || !tlp::g_isLegacyValues)
	{
		cocos2d::CCParticleSystem::setAngle(var);
		return;
	}

	if (getID() != "trailing-particles"_spr &&
		getID() != "ship-click-particles"_spr)
	{
		cocos2d::CCParticleSystem::setAngle(var);
	}
}

void TLPCCParticleSystem::setAngleVar(float var)
{
	if (!tlp::g_isModEnabled || !tlp::g_isLegacyValues)
	{
		cocos2d::CCParticleSystem::setAngleVar(var);
		return;
	}

	if (getID() != "trailing-particles"_spr &&
		getID() != "ship-click-particles"_spr)
	{
		cocos2d::CCParticleSystem::setAngleVar(var);
	}
}

void TLPCCParticleSystem::setGravity(const cocos2d::CCPoint& g)
{
	if (!tlp::g_isModEnabled || !tlp::g_isLegacyValues)
	{
		cocos2d::CCParticleSystem::setGravity(g);
		return;
	}

	if (getID() != "trailing-particles"_spr &&
		getID() != "ship-click-particles"_spr)
	{
		cocos2d::CCParticleSystem::setGravity(g);
	}
}

void TLPCCParticleSystem::setPosVar(cocos2d::CCPoint const& var)
{
	if (!tlp::g_isModEnabled || !tlp::g_isLegacyValues)
	{
		cocos2d::CCParticleSystem::setPosVar(var);
		return;
	}

	if (getID() != "player-ground-particles"_spr &&
		getID() != "trailing-particles"_spr &&
		getID() != "ship-click-particles"_spr)
	{
		cocos2d::CCParticleSystem::setPosVar(var);
	}
}

void TLPCCParticleSystem::setSpeed(float speed)
{
	if (!tlp::g_isModEnabled || !tlp::g_isLegacyValues)
	{
		cocos2d::CCParticleSystem::setSpeed(speed);
		return;
	}

	if (getID() != "player-ground-particles"_spr &&
		getID() != "trailing-particles"_spr &&
		getID() != "ship-click-particles"_spr)
	{
		cocos2d::CCParticleSystem::setSpeed(speed);
	}
}

void TLPCCParticleSystem::setSpeedVar(float speed)
{
	if (!tlp::g_isModEnabled || !tlp::g_isLegacyValues)
	{
		cocos2d::CCParticleSystem::setSpeedVar(speed);
		return;
	}

	if (getID() != "player-ground-particles"_spr &&
		getID() != "trailing-particles"_spr &&
		getID() != "ship-click-particles"_spr)
	{
		cocos2d::CCParticleSystem::setSpeedVar(speed);
	}
}
