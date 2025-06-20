#include <Geode/modify/PlayerObject.hpp>
#include <Geode/Geode.hpp>
#include "AlwaysSpawn.hpp"
#include "util.hpp"
#include "../variables/settings.hpp"

using namespace geode::prelude;

std::array<bool, 2> grounded = { false, false };
std::array<bool, 2> gate = { false, false };
std::array<int, 2> landSwitch = { 0, 0 };

void alwaysSpawnFunc(PlayerObject* player) {
	if (!alwaysSpawn)
		return;

	if (grounded[index(player)]) {
		if (!gate[index(player)]) {
			landSwitch[index(player)]++;
			float deviator = player->m_isUpsideDown ? 15 * player->m_vehicleSize : -15 * player->m_vehicleSize;

			if (!player->isVisible())
				return;

			if (landSwitch[index(player)] % 2 == 0) {
				player->m_landParticles1->resumeSystem();
				player->m_landParticles1->setPosition(player->getPositionX(), player->getPositionY() + deviator);
			}
			else {
				player->m_landParticles0->resumeSystem();
				player->m_landParticles0->setPosition(player->getPositionX(), player->getPositionY() + deviator);
			}

			gate[index(player)] = true;
		}
	}
	else
		gate[index(player)] = false;
}