#include <Geode/modify/PlayerObject.hpp>
#include "../globals/settings.hpp"
#include "../globals/particle.hpp"
#include "../util.hpp"

using namespace geode::prelude;

class $modify(PlayerObject) {
	virtual void update(float p0) {
		if (!isValid(this, true) || !g_isLegacyScaling) return PlayerObject::update(p0);
		PlayerObject::update(p0);

		for (auto system : CCArrayExt<CCParticleSystem*>(this->m_particleSystems)) {
			if (this->m_vehicleSize != 1.f)
				system->loadScaledDefaults(1.f);

			system->setScale(this->m_vehicleSize);
		}
	}
};