#include <tlp/PlayerObject.hpp>
#include <tlp/CCParticleSystem.hpp>
#include <tlp/Settings.hpp>

bool TLPPlayerObject::init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer)
{
	if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
	{
		return false;
	}

	if (!tlp::g_isModEnabled ||
		(!tlp::g_worksForP2 && m_isSecondPlayer) ||
		(!tlp::g_worksForP1 && !m_isSecondPlayer))
	{
		return true;
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

	m_fields->allParticles.push_back(m_playerGroundParticles);
	m_fields->allParticles.push_back(m_trailingParticles);
	m_fields->allParticles.push_back(m_shipClickParticles);
	m_fields->allParticles.push_back(m_vehicleGroundParticles);
	m_fields->allParticles.push_back(m_ufoClickParticles);
	m_fields->allParticles.push_back(m_robotBurstParticles);
	m_fields->allParticles.push_back(m_dashParticles);
	m_fields->allParticles.push_back(m_swingBurstParticles1);
	m_fields->allParticles.push_back(m_swingBurstParticles2);
	m_fields->allParticles.push_back(m_landParticles0);
	m_fields->allParticles.push_back(m_landParticles1);

	return true;
}

void TLPPlayerObject::toggleRobotMode(bool enable, bool noEffects)
{
	PlayerObject::toggleRobotMode(enable, noEffects);

	if (tlp::g_isModEnabled && tlp::g_isLegacyValues && m_isRobot)
	{
		m_playerGroundParticles->setID("");
		m_playerGroundParticles->setPosVar(cocos2d::CCPoint(15.f, 0.f));
		m_playerGroundParticles->setID("player-ground-particles"_spr);
	}
}

void TLPPlayerObject::toggleSpiderMode(bool enable, bool noEffects)
{
	PlayerObject::toggleSpiderMode(enable, noEffects);

	if (tlp::g_isModEnabled && tlp::g_isLegacyValues && m_isSpider)
	{
		m_playerGroundParticles->setID("");
		m_playerGroundParticles->setPosVar(cocos2d::CCPoint(15.f, 0.f));
		m_playerGroundParticles->setID("player-ground-particles"_spr);
	}
}

void TLPPlayerObject::update(float dt)
{
	if (!tlp::g_isModEnabled ||
		(!tlp::g_worksForP2 && m_isSecondPlayer) ||
		(!tlp::g_worksForP1 && !m_isSecondPlayer))
	{
		PlayerObject::update(dt);
		return;
	}

	if (tlp::g_isLegacyTracking)
	{
		cocos2d::CCPoint posFactor = cocos2d::CCPoint(10.f, m_isUpsideDown ? -13.f : 13.f) * m_vehicleSize;
		m_playerGroundParticles->setPosition(getPosition() - posFactor);
	}

	if (tlp::g_isLegacyScaling && m_fields->lastVehicleScale != m_vehicleSize)
	{
		for (auto& particle : m_fields->allParticles)
		{
			particle->setScale(m_vehicleSize);
		}

		m_fields->lastVehicleScale = m_vehicleSize;
	}

	PlayerObject::update(dt);
}