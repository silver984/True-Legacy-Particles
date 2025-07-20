#include <Geode/modify/PlayerObject.hpp>
#include "../util.hpp"
#include "../globals/settings.hpp"

class $modify(PlayerObject) {
	void update(float p0) override {
		if (!isValid(this, true)) return PlayerObject::update(p0);
		PlayerObject::update(p0);
		if (g_centerDashX) m_dashParticles->setPositionX(this->getPositionX());
		if (g_centerDashY) m_dashParticles->setPositionY(this->getPositionY());
	}
};