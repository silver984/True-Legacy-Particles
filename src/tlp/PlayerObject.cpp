#include <tlp/PlayerObject.hpp>
#include <tlp/CCParticleSystem.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Log.hpp>
#include <string>

bool TLPPlayerObject::init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer) {
	if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer)) {
		return false;
	}

	m_playerGroundParticles->setID("player-ground-particles"_spr);
	m_trailingParticles->setID("trailing-particles"_spr);
	m_shipClickParticles->setID("ship-click-particles"_spr);
	m_vehicleGroundParticles->setID("vehicle-ground-particles"_spr);
	m_ufoClickParticles->setID("ufo-click-particles"_spr);
	m_robotBurstParticles->setID("robot-burst-particles"_spr);
	m_dashParticles->setID("dash-particles"_spr);
	m_swingBurstParticles1->setID("swing-burst-particles-1"_spr);
	m_swingBurstParticles2->setID("swing-burst-particles-2"_spr);
	m_landParticles0->setID("land-particles-0"_spr);
	m_landParticles1->setID("land-particles-1"_spr);

	auto& allParticles = m_fields->allParticles;
	allParticles.push_back(m_playerGroundParticles);
	allParticles.push_back(m_trailingParticles);
	allParticles.push_back(m_shipClickParticles);
	allParticles.push_back(m_vehicleGroundParticles);
	allParticles.push_back(m_ufoClickParticles);
	allParticles.push_back(m_robotBurstParticles);
	allParticles.push_back(m_dashParticles);
	allParticles.push_back(m_swingBurstParticles1);
	allParticles.push_back(m_swingBurstParticles2);
	allParticles.push_back(m_landParticles0);
	allParticles.push_back(m_landParticles1);
	m_fields->landParticles.push_back(m_landParticles0);
	m_fields->landParticles.push_back(m_landParticles1);

	return true;
}

void TLPPlayerObject::onGround() {
	if (!isVisible()) {
		return;
	}

	auto& currentIndex = m_fields->currentLandParticle;
	currentIndex = (currentIndex + 1) % 2;
	auto& current = m_fields->landParticles[currentIndex];

	cocos2d::CCPoint posFactor = cocos2d::CCPoint(0.f, (m_height / 2.f) * m_vehicleSize);
	posFactor.y = m_isUpsideDown ? -posFactor.y : posFactor.y;

	modifyParticle(
		current,
		[&]() {
			current->setPosition(getPosition() - posFactor);
			current->setRotation(m_isUpsideDown ? 180.f : 0.f);
			current->resetSystem();
		}
	);
}

void TLPPlayerObject::toggleRobotMode(bool enable, bool noEffects) {
	PlayerObject::toggleRobotMode(enable, noEffects);

	if (enable) {
		modifyParticle(
			m_playerGroundParticles,
			[this]() {
				m_playerGroundParticles->setPosVar(cocos2d::CCPoint(15.f, 0.f));
			}
		);
	}
}

void TLPPlayerObject::toggleSpiderMode(bool enable, bool noEffects) {
	PlayerObject::toggleSpiderMode(enable, noEffects);

	if (enable) {
		modifyParticle(
			m_playerGroundParticles,
			[this]() {
				m_playerGroundParticles->setPosVar(cocos2d::CCPoint(15.f, 0.f));
			}
		);
	}
}

void TLPPlayerObject::update(float dt) {
	cocos2d::CCPoint posFactor = cocos2d::CCPoint(
		10.f,
		m_isUpsideDown ? -13.f : 13.f
	) * m_vehicleSize;
	
	posFactor.y = !m_isOnGround3 ? -posFactor.y : posFactor.y;
	m_playerGroundParticles->setPosition(getPosition() - posFactor);

	// on player resize
	if (m_fields->lastVehicleScale != m_vehicleSize) {
		for (auto& particle : m_fields->allParticles) {
			particle->setScale(m_vehicleSize);
		}

		m_fields->lastVehicleScale = m_vehicleSize;
	}

	// on player hit ground
	if (m_isOnGround) {
		if (m_fields->hasTouchedGround != m_isOnGround) {
			onGround();
			m_fields->hasTouchedGround = m_isOnGround;
		}
	} else {
		m_fields->hasTouchedGround = false;
	}

	PlayerObject::update(dt);
}

void TLPPlayerObject::modifyParticle(cocos2d::CCParticleSystemQuad* particle, std::function<void()> callback) {
	std::string lastID = particle->getID();
	particle->setID("");
	callback();
	particle->setID(lastID);
}