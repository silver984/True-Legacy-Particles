#include <Geode/modify/PlayerObject.hpp>
#include "LegacyTracking.hpp"
#include "util.hpp"
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"

using namespace geode::prelude;

void tracking(PlayerObject* player) {
	if (!legacyTracking)
		return;

	int i = index(player);

	if (particle[i].isGrounded) {
		extra[i].onGroundPos = player->m_playerGroundParticles->getPosition();
		CCPoint air = extra[i].onGroundPos - player->getPosition();
		extra[i].offGroundPos = air;
	}

	if (particle[i].spiderDashed) {
		if (particle[i].isGrounded)
			particle[i].spiderDashed = false;
	}

	if (!particle[i].spiderDashed) {
		bool bannedModes =
			player->m_isShip ||
			player->m_isBird ||
			player->m_isDart ||
			player->m_isSwing;

		if (!bannedModes) {
			CCPoint newPos = CCPoint(
				player->getPosition() +
				extra[i].offGroundPos
			);

			player->m_playerGroundParticles->setPosition(newPos);
		}
		else
			player->m_playerGroundParticles->setPosition(CCPoint(0, 0));
	}
	else
		player->m_playerGroundParticles->setPosition(CCPoint(0, 0));
}