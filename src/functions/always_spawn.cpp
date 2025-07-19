#include <Geode/modify/PlayerObject.hpp>
#include "../globals/settings.hpp"
#include "../globals/particle.hpp"
#include "../util.hpp"

using namespace geode::prelude;

class $modify(PlayerObject) {
	virtual void update(float p0) {
		if (!isValid(this, true) || !g_isAlwaysSpawn) return PlayerObject::update(p0);

		int upsideDown = this->m_isUpsideDown ? -1 : 1;
		int sideways = this->m_isSideways ? -1 : 1;
		int goingLeft = this->m_isGoingLeft ? -1 : 1;
		int i = playerIndex(this);

		g_land[i].gravity = CCPoint(g_landGravity.x * (goingLeft * sideways), g_landGravity.y * (upsideDown * sideways));
		g_land[i].angle = g_landAngle * (upsideDown * sideways);

		if (!this->isVisible())
			return PlayerObject::update(p0);

		if (g_particle[i].isGrounded) {
			if (!g_land[i].gate) {
				g_land[i].landSwitch++;

				CCPoint deviate;
				if (this->m_isSideways) {
					if (!this->m_isUpsideDown)
						deviate.x = -(this->m_height / 2) * this->m_vehicleSize;
					else
						deviate.x = (this->m_height / 2) * this->m_vehicleSize;
				}
				else
					deviate.x = 0;

				if (!this->m_isSideways) {
					if (this->m_isUpsideDown)
						deviate.y = (this->m_height / 2) * this->m_vehicleSize;
					else
						deviate.y = -(this->m_height / 2) * this->m_vehicleSize;
				}
				else
					deviate.y = 0;

				CCPoint pos = CCPoint(this->getPosition().x + deviate.x, this->getPosition().y + deviate.y);

				if (g_land[i].landSwitch % 2 == 0) {
					this->m_useLandParticles0 = false;
					this->m_landParticles0->setGravity(g_land[i].gravity);
					this->m_landParticles0->setAngle(g_land[i].angle);
					this->m_landParticles0->setPosition(pos);
					this->m_landParticles0->resetSystem();
				}
				else {
					this->m_useLandParticles0 = true;
					this->m_landParticles1->setGravity(g_land[i].gravity);
					this->m_landParticles1->setAngle(g_land[i].angle);
					this->m_landParticles1->setPosition(pos);
					this->m_landParticles1->resetSystem();
				}

				g_land[i].gate = true;
			}
		}
		else
			g_land[i].gate = false;

		PlayerObject::update(p0);
	}
};