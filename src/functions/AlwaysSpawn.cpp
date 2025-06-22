#include <Geode/modify/PlayerObject.hpp>
#include <Geode/Geode.hpp>
#include "AlwaysSpawn.hpp"
#include "util.hpp"
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"

using namespace geode::prelude;

std::array<bool, 2> grounded = { false, false };
std::array<bool, 2> gate = { false, false };
std::array<int, 2> landSwitch = { 0, 0 };
std::array<bool, 2> hadReset0 = { 0, 0 };
std::array<bool, 2> hadReset1 = { 0, 0 };

void alwaysSpawnFunc(PlayerObject* player) {
	if (!alwaysSpawn)
		return;

	int upsideDown = player->m_isUpsideDown ? -1 : 1;
	int sideways = player->m_isSideways ? -1 : 1;
	int goingLeft = player->m_isGoingLeft ? -1 : 1;
	land[index(player)].gravity = CCPoint(landGravity.x * (goingLeft * sideways), landGravity.y * (upsideDown * sideways));
	land[index(player)].angle = landAngle * (upsideDown * sideways);

	if (!player->isVisible())
		return;

	if (grounded[index(player)]) {
		if (!gate[index(player)]) {
			landSwitch[index(player)]++;

			float deviateX;
			if (player->m_isSideways) {
				if (!player->m_isUpsideDown)
					deviateX = -(player->m_height / 2) * player->m_vehicleSize;
				else
					deviateX = (player->m_height / 2) * player->m_vehicleSize;
			}
			else
				deviateX = 0;

			float deviateY;
			if (!player->m_isSideways) {
				if (player->m_isUpsideDown)
					deviateY = (player->m_height / 2) * player->m_vehicleSize;
				else
					deviateY = -(player->m_height / 2) * player->m_vehicleSize;
			}
			else
				deviateY = 0;

			CCPoint pos = CCPoint(player->getPosition().x + deviateX, player->getPosition().y + deviateY);

			if (landSwitch[index(player)] % 2 == 0) {
				player->m_useLandParticles0 = false;
				player->m_landParticles0->setGravity(land[index(player)].gravity);
				player->m_landParticles0->setAngle(land[index(player)].angle);
				player->m_landParticles0->setPosition(pos);
				player->m_landParticles0->resetSystem();
			}
			else {
				player->m_useLandParticles0 = true;
				player->m_landParticles1->setGravity(land[index(player)].gravity);
				player->m_landParticles1->setAngle(land[index(player)].angle);
				player->m_landParticles1->setPosition(pos);
				player->m_landParticles1->resetSystem();
			}

			gate[index(player)] = true;
		}
	}
	else
		gate[index(player)] = false;
}