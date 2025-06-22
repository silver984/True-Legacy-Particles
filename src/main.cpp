#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "functions/Core.hpp"
#include "functions/AlwaysSpawn.hpp"
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
		isTrueVals = Mod::get()->getSettingValue<bool>("true-vals");
		alwaysSpawn = Mod::get()->getSettingValue<bool>("always-spawn");
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
			CCDictionary* dragDict =
				CCDictionary::createWithContentsOfFile("dragEffect.plist");

			CCDictionary* landDict =
				CCDictionary::createWithContentsOfFile("landEffect.plist");

			sourceGravity = CCPoint(
				((CCString*)dragDict->objectForKey("gravityx"))->floatValue(),
				((CCString*)dragDict->objectForKey("gravityy"))->floatValue()
			);

			landGravity = CCPoint(
				((CCString*)landDict->objectForKey("gravityx"))->floatValue(),
				((CCString*)landDict->objectForKey("gravityy"))->floatValue()
			);
			landAngle = ((CCString*)landDict->objectForKey("angle"))->floatValue();

			// y for variance
			sourceAngle = CCPoint(
				((CCString*)dragDict->objectForKey("angle"))->floatValue(),
				((CCString*)dragDict->objectForKey("angleVariance"))->floatValue()
			);

			// y for variance
			sourceStartSize = CCPoint(
				((CCString*)dragDict->objectForKey("startParticleSize"))->floatValue(),
				((CCString*)dragDict->objectForKey("startParticleSizeVariance"))->floatValue()
			);

			// y for variance
			sourceSpeed = CCPoint(
				((CCString*)dragDict->objectForKey("speed"))->floatValue(),
				((CCString*)dragDict->objectForKey("speedVariance"))->floatValue()
			);

			sourcePosVar = CCPoint(
				((CCString*)dragDict->objectForKey("sourcePositionVariancex"))->floatValue(),
				((CCString*)dragDict->objectForKey("sourcePositionVariancey"))->floatValue()
			);

			drag[i].posVar = sourcePosVar;
			drag[i].speed = sourceSpeed.x;
			drag[i].speedVar = sourceSpeed.y;
			drag[i].startSize = sourceStartSize.x;
			drag[i].startSizeVar = sourceStartSize.y;

			trail[i].posVar = CCPoint(0, sourcePosVar.y * 0.4);
			trail[i].speed = sourceSpeed.x * 0.2;
			trail[i].speedVar = sourceSpeed.y * 0.2;

			shipClick[i].posVar = CCPoint(0, sourcePosVar.y * 0.4);
			shipClick[i].speed = sourceSpeed.x * 2;
			shipClick[i].speedVar = sourceSpeed.y * 2;
			shipClick[i].startSize = sourceStartSize.x * 1.5;
			shipClick[i].startSizeVar = sourceStartSize.y * 1.5;

			grounded[i] = 0;
			gate[i] = 0;
			landSwitch[i] = 0;
		}

		return true;
	}

	virtual void resetObject() {
		m_landParticles0->setRotation(0);
		m_landParticles1->setRotation(0);

		for (int i = 0; i < 2; i++) {
			grounded[i] = 0;
			gate[i] = 0;
			landSwitch[i] = 0;
		}
		PlayerObject::resetObject();
	}

	void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to) {
		particle[index(this)].spiderDashed = true;
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

	void hitGround(GameObject* p0, bool p1) {
		if (!p1)
			grounded[index(this)] = true;

		PlayerObject::hitGround(p0, p1);
	}

	virtual void update(float delta) {
		if (!check(this, true))
			return PlayerObject::update(delta);

		core(this);

		if (noRotation) {
			int angle = this->m_isSideways ? -90 : 0;
			m_landParticles0->setRotation(angle);
			m_landParticles1->setRotation(angle);
		}

		PlayerObject::update(delta);

		grounded[index(this)] = false;

		if (centerDashX)
			m_dashParticles->setPositionX(this->getPositionX());
		if (centerDashY)
			m_dashParticles->setPositionY(this->getPositionY());
	}
};