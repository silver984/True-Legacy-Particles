#pragma once

#include <Geode/modify/PlayerObject.hpp>
#include <vector>

struct TLPPlayerObject : geode::Modify<TLPPlayerObject, PlayerObject>
{
	bool init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer);
	void toggleRobotMode(bool enable, bool noEffects);
	void toggleSpiderMode(bool enable, bool noEffects);
	void update(float dt) override;

	struct Fields
	{
		float lastVehicleScale = 0.f;
		std::vector<cocos2d::CCParticleSystemQuad*> allParticles;
	};
};