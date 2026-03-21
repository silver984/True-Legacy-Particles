#pragma once

#include <Geode/modify/CCParticleSystem.hpp>
#include <Geode/loader/Mod.hpp>
#include <string>

struct TLPCCParticleSystem : geode::Modify<TLPCCParticleSystem, cocos2d::CCParticleSystem>
{
	void loadScaledDefaults(float);
	void setAngle(float var) override;
	void setAngleVar(float var) override;
	void setGravity(const cocos2d::CCPoint& g) override;
	void setPosVar(cocos2d::CCPoint const& var) override;
	void setSpeed(float speed) override;
	void setSpeedVar(float speed) override;
};