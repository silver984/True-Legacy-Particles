#include <Geode/modify/PlayerObject.hpp>
#include "globals/particle.hpp"
#include "globals/settings.hpp"
#include "util.hpp"

class $modify(PlayerObject) {
	void resetObject() override {
		m_landParticles0->setRotation(0);
		m_landParticles1->setRotation(0);
		PlayerObject::resetObject();
	}

	void update(float p0) override {
		if (!isValid(this, true)) return PlayerObject::update(p0);
		PlayerObject::update(p0);

		if (g_isNoRotation) {
			int angle = this->m_isSideways ? -90 : 0;
			m_landParticles0->setRotation(angle);
			m_landParticles1->setRotation(angle);
		}
	}
};