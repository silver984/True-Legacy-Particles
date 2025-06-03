#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "functions/Core.hpp"
#include "functions/util.hpp"
#include "functions/variance.hpp"
#include "variables/particle.hpp"
#include "variables/settings.hpp"

using namespace geode::prelude;

class $modify(PlayerObject) {
	bool init(int p0, int p1, GJBaseGameLayer * p2, cocos2d::CCLayer * p3, bool p4) {
		if (!PlayerObject::init(p0, p1, p2, p3, p4))
			return false;

		toggle = Mod::get()->getSettingValue<bool>("switch");
		legacyTracking = Mod::get()->getSettingValue<bool>("legacy-tracking");
		legacyScaling = Mod::get()->getSettingValue<bool>("legacy-scaling");
		inconstVal = Mod::get()->getSettingValue<bool>("inconst-values");
		noRotation = Mod::get()->getSettingValue<bool>("no-rotation");
		centerDash = Mod::get()->getSettingValue<bool>("center-dash");
		player1 = Mod::get()->getSettingValue<bool>("player-1");
		player2 = Mod::get()->getSettingValue<bool>("player-2");

		if (centerDash) {
			centerDashX = Mod::get()->getSettingValue<bool>("center-dash-x");
			centerDashY = Mod::get()->getSettingValue<bool>("center-dash-y");
		}
		else {
			centerDashX = false;
			centerDashY = false;
		}

		for (int i = 0; i < 2; i++) {
			trail[i].posVar = CCPoint(0, 2);
			trail[i].speed = 15;
			trail[i].speedVar = 4;

			shipClick[i].posVar = CCPoint(0, 2);
			shipClick[i].speed = 150;
			shipClick[i].speedVar = 40;
			shipClick[i].startSize = 6;
			shipClick[i].startSizeVar = 4.5;

			drag[i].posVar = drag[i].sourcePosVar;
		}

		return true;
	}

	void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to) {
		int i = index(this);
		particle[i].spiderDashed = true;
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

	virtual void update(float delta) {
		if (!check(this, true)) {
			PlayerObject::update(delta);
			return;
		}
		core(this);

		PlayerObject::update(delta);

		if (centerDashX)
			m_dashParticles->setPositionX(this->getPositionX());
		if (centerDashY)
			m_dashParticles->setPositionY(this->getPositionY());

		if (noRotation) {
			m_landParticles0->setRotation(0);
			m_landParticles1->setRotation(0);
		}
	}
};