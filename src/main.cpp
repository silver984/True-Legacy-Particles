#include "particle.hpp"
#include "data.hpp"

#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {
	void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to) {
		auto& p = Particle::get();
		int index;

		if (!m_isSecondPlayer)
			index = 0;
		else
			index = 1;

		p[index].spiderDashed = true;
		p[index].framesBeforeSpiderDashed = 0;

		PlayerObject::playSpiderDashEffect(from, to);
	}

	void togglePlayerScale(bool isMini, bool p1) {
		scale(isMini, m_isSecondPlayer);
		PlayerObject::togglePlayerScale(isMini, p1);
	}

	void toggleRobotMode(bool isRobot, bool p1) {
		wide(isRobot, m_isSecondPlayer);
		PlayerObject::toggleRobotMode(isRobot, p1);
	}

	void toggleSpiderMode(bool isSpider, bool p1) {
		wide(isSpider, m_isSecondPlayer);
		PlayerObject::toggleSpiderMode(isSpider, p1);
	}
};

class $modify(GJBaseGameLayer) {
	virtual bool init() {
		auto& d = Data::get();
		d.legacyTracking = Mod::get()->getSettingValue<bool>("legacy-tracking");
		d.legacyLand = Mod::get()->getSettingValue<bool>("legacy-land");
		d.legacyScaling = Mod::get()->getSettingValue<bool>("legacy-scaling");
		d.inconstVal = Mod::get()->getSettingValue<bool>("inconst-values");
		d.centerDash = Mod::get()->getSettingValue<bool>("center-dash");
		return GJBaseGameLayer::init();
	}

	void createPlayer() {
		reset();
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

		for (int i = 0; i < 2; i++) {
			core(player[i], i, delta);

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