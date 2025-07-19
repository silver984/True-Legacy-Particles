#include <Geode/modify/PlayerObject.hpp>
#include "../util.hpp"
#include "../globals/settings.hpp"

class $modify(PlayerObject) {
	virtual void update(float delta) {
		if (!isValid(this, true)) return PlayerObject::update(delta);
		PlayerObject::update(delta);
		if (g_centerDashX) m_dashParticles->setPositionX(this->getPositionX());
		if (g_centerDashY) m_dashParticles->setPositionY(this->getPositionY());
	}
};