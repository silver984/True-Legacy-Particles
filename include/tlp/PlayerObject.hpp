#pragma once
#include <Geode/modify/PlayerObject.hpp>
#include <vector>
#include <functional>

struct TLPPlayerObject : geode::Modify<TLPPlayerObject, PlayerObject> {
	bool init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer);
	// tlp addition
	void onGround();
	void toggleRobotMode(bool enable, bool noEffects);
	void toggleSpiderMode(bool enable, bool noEffects);
	void update(float dt) override;
	// tlp addition
	void modifyParticle(cocos2d::CCParticleSystemQuad* particle, std::function<void()> callback);

	struct Fields {
		float lastVehicleScale = 1.f;
		std::vector<cocos2d::CCParticleSystemQuad*> allParticles;
		std::vector<cocos2d::CCParticleSystemQuad*> landParticles;
		bool hasTouchedGround = false;
		int currentLandParticle = -1;
	};
};