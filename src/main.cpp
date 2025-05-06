#include "particle.hpp"
#include "data.hpp"

#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		Data::get().isInLevel = true;
		return PlayLayer::init(level, useReplay, dontCreateObjects);
	}

	void onQuit() {
		Data::get().isInLevel = false;
		PlayLayer::onQuit();
	}
};

class $modify(PlayerObject) {
	void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to) {
		auto& p = Particle::get();

		if (!m_isSecondPlayer) {
			p[0].spiderDashed = true;
			p[0].framesBeforeSpiderDashed = 0;
		}
		else {
			p[1].spiderDashed = true;
			p[1].framesBeforeSpiderDashed = 0;
		}

		PlayerObject::playSpiderDashEffect(from, to);
	}

	// structs are used below for parameters in order to not exceed 80 characters in one line of code
	// (as stated in the geode doc)

	void togglePlayerScale(bool isMini, bool p1) {
		auto& d = Data::get();
		ScaleParams params = {
			m_isSecondPlayer,
			d.isInLevel,
			d.legacyTracking,
			d.inconstVal,
			d.legacyScaling,
			isMini
		};
		scale(params);
		PlayerObject::togglePlayerScale(isMini, p1);
	}

	void toggleRobotMode(bool isRobot, bool p1) {
		auto& d = Data::get();
		WideParams params = {
			m_isSecondPlayer,
			d.legacyTracking,
			d.inconstVal,
			isRobot
		};
		wide(params);
		PlayerObject::toggleRobotMode(isRobot, p1);
	}

	void toggleSpiderMode(bool isSpider, bool p1) {
		auto& d = Data::get();
		WideParams params = {
			m_isSecondPlayer,
			d.legacyTracking,
			d.inconstVal,
			isSpider
		};
		wide(params);
		PlayerObject::toggleSpiderMode(isSpider, p1);
	}
};

class $modify(GJBaseGameLayer) {
	virtual bool init() {
		auto& d = Data::get();
		d.legacyTracking = Mod::get()->getSettingValue<bool>("legacy-tracking");
		d.legacyScaling = Mod::get()->getSettingValue<bool>("legacy-scaling");
		d.inconstVal = Mod::get()->getSettingValue<bool>("legacy-land");
		d.legacyLand = Mod::get()->getSettingValue<bool>("legacy-land");
		d.centerDash = Mod::get()->getSettingValue<bool>("center-dash");
		return GJBaseGameLayer::init();
	}

	void createPlayer() {
		reset(Data::get().isInLevel);
		GJBaseGameLayer::createPlayer();
	}

	virtual void update(float delta) {
		GJBaseGameLayer::update(delta);

		auto& d = Data::get();
		auto& p = Particle::get();

		PlayerObject* player[2]{
			m_player1,
			m_player2
		};

		CoreParams params1 = {
			m_player1,
			0,
			d.legacyTracking,
			d.inconstVal,
			d.legacyScaling,
			delta
		};
		core(params1);

		CoreParams params2 = {
			m_player2,
			1,
			d.legacyTracking,
			d.inconstVal,
			d.legacyScaling,
			delta
		};
		core(params2);

		for (int i = 0; i < 2; i++) {
			p[i].framesBeforeSpiderDashed++;

			if (d.centerDash)
				player[i]->m_dashParticles->setPositionY(player[i]->getPositionY());

			if (d.legacyLand) {
				player[i]->m_landParticles0->setRotation(0);
				player[i]->m_landParticles1->setRotation(0);
			}
		}
	}
};