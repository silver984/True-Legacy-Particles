#include <Geode/modify/PlayerObject.hpp>
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "LegacyScaling.hpp"
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"

using namespace geode::prelude;

void scaling(PlayerObject* player) {
	if (!legacyScaling)
		return;

	for (auto system : CCArrayExt<CCParticleSystem*>(player->m_particleSystems)) {
		if (player->m_vehicleSize != 1.f)
			system->loadScaledDefaults(1.f);

		system->setScale(player->m_vehicleSize);
	}
}