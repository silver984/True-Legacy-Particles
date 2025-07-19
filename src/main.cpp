#include <Geode/modify/PlayerObject.hpp>
#include "globals/particle.hpp"
#include "globals/settings.hpp"
#include "util.hpp"

class $modify(PlayerObject) {
	virtual void resetObject() {
		m_landParticles0->setRotation(0);
		m_landParticles1->setRotation(0);
		PlayerObject::resetObject();
	}

	virtual void update(float p0) {
		if (!isValid(this, true)) return PlayerObject::update(p0);
		PlayerObject::update(p0);

		if (g_isNoRotation) {
			int angle = this->m_isSideways ? -90 : 0;
			m_landParticles0->setRotation(angle);
			m_landParticles1->setRotation(angle);
		}
	}
};