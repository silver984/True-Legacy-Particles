#include <tlp/CCParticleSystem.hpp>

void TLPCCParticleSystem::loadScaledDefaults(float v) {
	if (std::string(getID()).starts_with(geode::Mod::get()->getID())) {
		return;
	}

	cocos2d::CCParticleSystem::loadScaledDefaults(v);
}

void TLPCCParticleSystem::resetSystem() {
	if (getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr) {
		return;
	}

	cocos2d::CCParticleSystem::resetSystem();
}

void TLPCCParticleSystem::setAngle(float var) {
	if (getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr || getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr) {
		return;
	}

	cocos2d::CCParticleSystem::setAngle(var);
}

void TLPCCParticleSystem::setAngleVar(float var) {
	if (getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr || getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr) {
		return;
	}

	cocos2d::CCParticleSystem::setAngleVar(var);
}

void TLPCCParticleSystem::setGravity(const cocos2d::CCPoint& g) {
	if (getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr || getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr) {
		return;
	}

	cocos2d::CCParticleSystem::setGravity(g);
}

void TLPCCParticleSystem::setPosVar(cocos2d::CCPoint const& var) {
	if (getID() == "player-ground-particles"_spr || getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr) {
		return;
	}

	cocos2d::CCParticleSystem::setPosVar(var);
}

void TLPCCParticleSystem::setSpeed(float speed) {
	if (getID() == "player-ground-particles"_spr || getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr) {
		return;
	}

	cocos2d::CCParticleSystem::setSpeed(speed);
}

void TLPCCParticleSystem::setSpeedVar(float speed) {
	if (getID() == "player-ground-particles"_spr || getID() == "trailing-particles"_spr || getID() == "ship-click-particles"_spr) {
		return;
	}

	cocos2d::CCParticleSystem::setSpeedVar(speed);
}
