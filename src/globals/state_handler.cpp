#include <Geode/modify/PlayerObject.hpp>
#include "../functions/variance.hpp"
#include "../util.hpp"
#include "settings.hpp"
#include "particle.hpp"

class $modify(PlayerObject) {
	bool init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer) {
		if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
			return false;

		g_isModEnabled = Mod::get()->getSettingValue<bool>("switch");
		g_isLegacyTracking = Mod::get()->getSettingValue<bool>("legacy-tracking");
		g_isLegacyScaling = Mod::get()->getSettingValue<bool>("legacy-scaling");
		g_isTrueValues = Mod::get()->getSettingValue<bool>("true-vals");
		g_isAlwaysSpawn = Mod::get()->getSettingValue<bool>("always-spawn");
		g_isNoRotation = Mod::get()->getSettingValue<bool>("no-rotation");
		g_affectP1 = Mod::get()->getSettingValue<bool>("player-1");
		g_affectP2 = Mod::get()->getSettingValue<bool>("player-2");
		g_centerDashX = Mod::get()->getSettingValue<bool>("center-dash") ? Mod::get()->getSettingValue<bool>("center-dash-x") : false;
		g_centerDashY = Mod::get()->getSettingValue<bool>("center-dash") ? Mod::get()->getSettingValue<bool>("center-dash-y") : false;

		return true;
	}

	void resetObject() override {
		int i = playerIndex(this);
		g_particle[i].isGrounded = false;
		g_land[i].gate = false;
		g_land[i].landSwitch = 0;

		PlayerObject::resetObject();
	}

	void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to) {
		g_particle[playerIndex(this)].spiderDashed = true;
		PlayerObject::playSpiderDashEffect(from, to);
	}

	void togglePlayerScale(bool isMini, bool p1) {
		scale(this, isMini);
		PlayerObject::togglePlayerScale(isMini, p1);
	}

	void toggleRobotMode(bool isRobot, bool p1) {
		wide(this, isRobot);
		PlayerObject::toggleRobotMode(isRobot, p1);
	}

	void toggleSpiderMode(bool isSpider, bool p1) {
		wide(this, isSpider);
		PlayerObject::toggleSpiderMode(isSpider, p1);
	}

	void hitGround(GameObject* p0, bool hitCeiling) {
		if (!hitCeiling) g_particle[playerIndex(this)].isGrounded = true;
		PlayerObject::hitGround(p0, hitCeiling);
	}

	void update(float p0) override {
		PlayerObject::update(p0);
		g_particle[playerIndex(this)].isGrounded = false;
	}
};