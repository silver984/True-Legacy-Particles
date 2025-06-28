#include <Geode/modify/PlayerObject.hpp>
#include <Geode/Geode.hpp>
#include "AlwaysSpawn.hpp"
#include "util.hpp"
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"

using namespace geode::prelude;

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
	int i = index(player);

	land[i].gravity = CCPoint(landGravity.x * (goingLeft * sideways), landGravity.y * (upsideDown * sideways));
	land[i].angle = landAngle * (upsideDown * sideways);

	if (!player->isVisible())
		return;

	if (particle[i].isGrounded) {
		if (!gate[i]) {
			landSwitch[i]++;

			CCPoint deviate;
			if (player->m_isSideways) {
				if (!player->m_isUpsideDown)
					deviate.x = -(player->m_height / 2) * player->m_vehicleSize;
				else
					deviate.x = (player->m_height / 2) * player->m_vehicleSize;
			}
			else
				deviate.x = 0;

			if (!player->m_isSideways) {
				if (player->m_isUpsideDown)
					deviate.y = (player->m_height / 2) * player->m_vehicleSize;
				else
					deviate.y = -(player->m_height / 2) * player->m_vehicleSize;
			}
			else
				deviate.y = 0;
			
			CCPoint pos = CCPoint(player->getPosition().x + deviate.x, player->getPosition().y + deviate.y);

			if (landSwitch[i] % 2 == 0) {
				player->m_useLandParticles0 = false;
				player->m_landParticles0->setGravity(land[i].gravity);
				player->m_landParticles0->setAngle(land[i].angle);
				player->m_landParticles0->setPosition(pos);
				player->m_landParticles0->resetSystem();
			}
			else {
				player->m_useLandParticles0 = true;
				player->m_landParticles1->setGravity(land[i].gravity);
				player->m_landParticles1->setAngle(land[i].angle);
				player->m_landParticles1->setPosition(pos);
				player->m_landParticles1->resetSystem();
			}

			gate[i] = true;
		}
	}
	else
		gate[i] = false;
}