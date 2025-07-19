#include <Geode/modify/PlayerObject.hpp>
#include "../globals/settings.hpp"
#include "../globals/particle.hpp"
#include "../util.hpp"

class $modify(PlayerObject) {
	virtual void update(float p0) {
		if (!isValid(this, true) || !g_isLegacyTracking) return PlayerObject::update(p0);

		int i = playerIndex(this);
		if (g_particle[i].spiderDashed) {
			if (g_particle[i].isGrounded)
				g_particle[i].spiderDashed = false;
		}

		if (!g_particle[i].spiderDashed) {
			bool bannedModes =
				this->m_isShip ||
				this->m_isBird ||
				this->m_isDart ||
				this->m_isSwing;

			if (!bannedModes) {
				int mult1 = this->m_isGoingLeft ? -1 : 1;
				int mult2 = this->m_isUpsideDown ? -1 : 1;
				int mult3 = this->m_isBall ? -1 : 1;
				int mult4 = this->m_isOnGround3 ? -1 : 1;
				int tspmo;
				if (this->m_isBall)
					tspmo = (mult2 * (mult3 * mult4));
				else
					tspmo = mult2;

				float x = this->getPosition().x + ((-10.f * mult1) * this->m_vehicleSize);
				float y = this->getPosition().y + ((-13.f * tspmo * this->m_vehicleSize));
				CCPoint newPos = CCPoint(x, y);
				this->m_playerGroundParticles->setPosition(newPos);
			}
			else
				this->m_playerGroundParticles->setPosition(CCPoint(0, 0));
		}
		else
			this->m_playerGroundParticles->setPosition(CCPoint(0, 0));

		PlayerObject::update(p0);
	}
};