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

template <typename T>
T getSettingValue(std::string_view name) {
	static T value = (listenForSettingChangesV3<T>(name, [](T val) {
		value = val;
	}), Mod::get()->getSettingValue<T>(name));
	return value;
}

class $modify(GJBaseGameLayer) {
	void createPlayer() {
		auto& d = Data::get();
		reset(d.isInLevel);
		GJBaseGameLayer::createPlayer();
	}

	virtual void update(float delta) {
		auto& d = Data::get();
		auto& p = Particle::get();
		d.legacyTracking = getSettingValue<bool>("legacy-tracking");
		d.legacyScaling = getSettingValue<bool>("legacy-scaling");
		d.inconstVal = getSettingValue<bool>("inconst-values");

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
			// player[i]->m_dashParticles->setPosition(player[i]->m_obPosition); TO-DO! for some reason this actually doesnt center the dash particles
			player[i]->m_landParticles0->setRotation(0);
			player[i]->m_landParticles1->setRotation(0);
		}

		GJBaseGameLayer::update(delta);
	}
};