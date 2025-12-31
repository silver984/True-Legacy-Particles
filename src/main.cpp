#include <Geode/modify/PlayerObject.hpp>
#include <Geode/Enums.hpp>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/cocoa/CCDictionary.h>
#include <Geode/cocos/cocoa/CCString.h>
#include <Geode/cocos/particle_nodes/CCParticleSystem.h>
#include <Geode/utils/cocos.hpp>

namespace tlp
{
	struct Settings
	{
		bool isModEnabled = false;
		bool isLegacyTracking = false;
		bool isLegacyScaling = false;
		bool isLegacyValues = false;
		bool isLegacyRotation = false;
		bool worksForP1 = false;
		bool worksForP2 = false;
	} settings;

	struct ParticleBase
	{
		float speed = 0.f;
		float speedVar = 0.f;

		float startSize = 0.f;
		float startSizeVar = 0.f;

		float endSize = 0.f;
		float endSizeVar = 0.f;

		float angle = 0.f;
		float angleVar = 0.f;

		cocos2d::CCPoint posVar = cocos2d::CCPoint(0, 0);

		cocos2d::CCPoint gravity = cocos2d::CCPoint(0, 0);
	};
}

namespace
{
	enum PlayerGround
	{
		Ceiling = 0,
		Floor = 1,
		LeftWall = 2,
		RightWall = 3
	};

	void copyValues(tlp::ParticleBase& child, tlp::ParticleBase& parent)
	{
		child.angle = parent.angle;
		child.endSize = parent.endSize;
		child.endSizeVar = parent.endSizeVar;
		child.gravity = parent.gravity;
		child.posVar = parent.posVar;
		child.speed = parent.speed;
		child.speedVar = parent.speedVar;
		child.startSize = parent.startSize;
		child.startSizeVar = parent.startSizeVar;
	}

	void updateValues(cocos2d::CCParticleSystemQuad*& particle, tlp::ParticleBase& base, float scale, bool isUpsideDown, bool isGroundWall)
	{
		float curAngle = (isUpsideDown ? -base.angle : base.angle) - (isGroundWall ? 90.f : 0.f);
		
		particle->setAngle(curAngle);
		particle->setAngleVar(base.angleVar * scale);
		particle->setEndSize(base.endSize * scale);
		particle->setEndSizeVar(base.endSizeVar * scale);
		
		float a = 0.f;
		cocos2d::CCPoint curGravity = cocos2d::CCPoint();

		if (!isGroundWall)
		{
			a = isUpsideDown ? -base.gravity.y : base.gravity.y;
			curGravity = cocos2d::CCPoint(base.gravity.x, a);
		}
		else
		{
			a = isUpsideDown ? -base.gravity.x : base.gravity.x;
			curGravity = cocos2d::CCPoint(a, base.gravity.y);
		}

		particle->setGravity(curGravity);
		particle->setPosVar(cocos2d::CCPoint(base.posVar.x * scale, base.posVar.y * scale));
		particle->setSpeed(base.speed * scale);
		particle->setSpeedVar(base.speedVar * scale);
		particle->setStartSize(base.startSize * scale);
		particle->setStartSizeVar(base.startSizeVar * scale);
	}

	bool isPlayerEnabled(bool isSecondPlayer)
	{
		if (isSecondPlayer)
		{
			if (!tlp::settings.worksForP2)
			{
				return false;
			}

			return true;
		}

		if (!tlp::settings.worksForP1)
		{
			return false;
		}

		return true;
	}

	void widenedPosVar(tlp::ParticleBase& base, bool condition, bool isSecondPlayer)
	{
		if (tlp::settings.isModEnabled && condition)
		{
			if (!::isPlayerEnabled(isSecondPlayer))
			{
				return;
			}

			base.posVar = cocos2d::CCPoint(15.f, 0.f);
		}
	}

	bool isGroundWall(unsigned int ground)
	{
		ground &= 3;

		if (ground == ::PlayerGround::LeftWall || ground == ::PlayerGround::RightWall)
		{
			return true;
		}

		return false;
	}

	void swapPointValues(cocos2d::CCPoint& a)
	{
		cocos2d::CCPoint old = a;
		a = cocos2d::CCPoint(old.y, old.x);
	}

	void rotateValues(tlp::ParticleBase& base)
	{
		::swapPointValues(base.posVar);
		::swapPointValues(base.gravity);
	}
}

class $modify(PlayerObject)
{
	struct Fields
	{
		tlp::ParticleBase dragParticle;
		tlp::ParticleBase trailParticle;
		tlp::ParticleBase shipClickParticle;
		std::array<tlp::ParticleBase, 2u> landParticles{};
		std::array<cocos2d::CCParticleSystemQuad*, 2u> landParticleObjects{};
		cocos2d::CCPoint orgPosVar = cocos2d::CCPoint(0.f, 0.f);
		std::vector<std::string> ids;
		float lastVehicleSize = 0.f;
		float sinceGrounded = 0.f;
		float groundedElapsed = 0.f;
		bool isOnGround = false;
		bool hasHitGround = false;
		bool valuesShifted = false;
		bool playedSpiderDashEffect = false;
		unsigned int curLand : 1 = 1u;
		unsigned int ground : 2 = 0u;
		unsigned int lastGround : 2 = 0u;
	};

	bool init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer)
	{
		if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
		{
			return false;
		}

		typedef geode::prelude::Mod mod;

		tlp::settings.isModEnabled = mod::get()->getSettingValue<bool>("switch");

		if (!tlp::settings.isModEnabled)
		{
			return true;
		}

		tlp::settings.worksForP1 = mod::get()->getSettingValue<bool>("player-1");
		tlp::settings.worksForP2 = mod::get()->getSettingValue<bool>("player-2");
		
		if (!::isPlayerEnabled(m_isSecondPlayer))
		{
			return true;
		}

		tlp::settings.isLegacyTracking = mod::get()->getSettingValue<bool>("legacy-tracking");
		tlp::settings.isLegacyScaling = mod::get()->getSettingValue<bool>("legacy-scaling");
		tlp::settings.isLegacyValues = mod::get()->getSettingValue<bool>("legacy-values");
		tlp::settings.isLegacyRotation = mod::get()->getSettingValue<bool>("legacy-rotation");

		if (tlp::settings.isLegacyScaling)
		{
			std::string dragId = "drag_particle_p" + std::string(m_isSecondPlayer ? "2" : "1");
			std::string trailingId = "trailing_particle_p" + std::string(m_isSecondPlayer ? "2" : "1");
			std::string shipClickID = "ship_click_particle_p" + std::string(m_isSecondPlayer ? "2" : "1");

			m_fields->ids.push_back(dragId);
			m_fields->ids.push_back(trailingId);
			m_fields->ids.push_back(shipClickID);

			m_playerGroundParticles->setID(dragId);
			m_trailingParticles->setID(trailingId);
			m_shipClickParticles->setID(shipClickID);
		}

		m_fields->landParticleObjects[0] = m_landParticles0;
		m_fields->landParticleObjects[1] = m_landParticles1;

		cocos2d::CCDictionary* landDict = cocos2d::CCDictionary::createWithContentsOfFile("landEffect.plist");

		for (auto& it : m_fields->landParticles)
		{
			it.angle = ((cocos2d::CCString*)landDict->objectForKey("angle"))->floatValue();
			it.gravity = cocos2d::CCPoint(
				((cocos2d::CCString*)landDict->objectForKey("gravityx"))->floatValue(),
				((cocos2d::CCString*)landDict->objectForKey("gravityy"))->floatValue());
		}

		auto& dragParticle = m_fields->dragParticle;
		auto& trailParticle = m_fields->trailParticle;
		auto& shipClickParticle = m_fields->shipClickParticle;

		cocos2d::CCDictionary* dragDict = cocos2d::CCDictionary::createWithContentsOfFile("dragEffect.plist");

		dragParticle.angle = ((cocos2d::CCString*)dragDict->objectForKey("angle"))->floatValue();
		dragParticle.angleVar = ((cocos2d::CCString*)dragDict->objectForKey("angleVariance"))->floatValue();
		dragParticle.endSize = ((cocos2d::CCString*)dragDict->objectForKey("finishParticleSize"))->floatValue();
		dragParticle.endSizeVar = ((cocos2d::CCString*)dragDict->objectForKey("finishParticleSizeVariance"))->floatValue();
		
		dragParticle.gravity = cocos2d::CCPoint(((cocos2d::CCString*)dragDict->objectForKey("gravityx"))->floatValue(),
												((cocos2d::CCString*)dragDict->objectForKey("gravityy"))->floatValue());
		
		dragParticle.posVar = cocos2d::CCPoint(((cocos2d::CCString*)dragDict->objectForKey("sourcePositionVariancex"))->floatValue(),
											   ((cocos2d::CCString*)dragDict->objectForKey("sourcePositionVariancey"))->floatValue());
		
		m_fields->orgPosVar = dragParticle.posVar;

		if (!tlp::settings.isLegacyValues)
		{
			::swapPointValues(dragParticle.posVar);
		}
		
		dragParticle.speed = ((cocos2d::CCString*)dragDict->objectForKey("speed"))->floatValue();
		dragParticle.speedVar = ((cocos2d::CCString*)dragDict->objectForKey("speedVariance"))->floatValue();
		dragParticle.startSize = ((cocos2d::CCString*)dragDict->objectForKey("startParticleSize"))->floatValue();
		dragParticle.startSizeVar = ((cocos2d::CCString*)dragDict->objectForKey("startParticleSizeVariance"))->floatValue();

		::copyValues(trailParticle, dragParticle);
		if (tlp::settings.isLegacyValues)
		{
			trailParticle.posVar = cocos2d::CCPoint(0, trailParticle.posVar.y * 0.4f);
			trailParticle.speed = trailParticle.speed * 0.2f;
			trailParticle.speedVar = trailParticle.speedVar * 0.2f;
		}

		::copyValues(shipClickParticle, dragParticle);
		if (tlp::settings.isLegacyValues)
		{
			shipClickParticle.posVar = cocos2d::CCPoint(0, shipClickParticle.posVar.y * 0.4f);
			shipClickParticle.speed = shipClickParticle.speed * 2;
			shipClickParticle.speedVar = shipClickParticle.speedVar * 2;
			shipClickParticle.startSize = shipClickParticle.startSize * 1.5f;
			shipClickParticle.startSizeVar = shipClickParticle.startSizeVar * 1.5f;
			shipClickParticle.endSize = shipClickParticle.endSize * 1.5f;
			shipClickParticle.endSizeVar = shipClickParticle.endSizeVar * 1.5f;
		}

		return true;
	}

	void resetObject() override
	{
		PlayerObject::resetObject();

		if (tlp::settings.isModEnabled && tlp::settings.isLegacyRotation)
		{
			if (!::isPlayerEnabled(m_isSecondPlayer))
			{
				return;
			}

			m_fields->sinceGrounded = 0.f;

			for (auto& it : m_fields->landParticleObjects)
			{
				it->setRotation(0.f);
			}
		}
	}

	void update(float delta) override
	{
		if (tlp::settings.isModEnabled && ::isPlayerEnabled(m_isSecondPlayer))
		{
			if (m_fields->isOnGround)
			{
				if (!m_fields->hasHitGround)
				{
					if (isVisible() && m_fields->sinceGrounded >= 8.f)
					{
						m_fields->curLand++;

						float yFactor = (m_height * m_vehicleSize) / 2;
						float yOffset = m_isUpsideDown ? yFactor : -yFactor;
						float rotation = 0.f;
						float angle = m_fields->landParticles[m_fields->curLand].angle;
						float curAngle = m_isUpsideDown ? -angle : angle;

						cocos2d::CCPoint pos = cocos2d::CCPoint();
						cocos2d::CCPoint gravity = m_fields->landParticles[m_fields->curLand].gravity;
						cocos2d::CCPoint curGravity = gravity;
						curGravity.y = m_isUpsideDown ? -gravity.y : gravity.y;

						if (!::isGroundWall(m_fields->ground))
						{
							pos = cocos2d::CCPoint(getPosition().x, getPosition().y + yOffset);
						}

						if (::isGroundWall(m_fields->ground))
						{
							pos = cocos2d::CCPoint(getPosition().x + yOffset, getPosition().y);
							rotation = 90.f;
						}

						m_useLandParticles0 = m_fields->curLand % 2 != 0;
						m_fields->landParticleObjects[m_fields->curLand]->setPosition(pos);
						m_fields->landParticleObjects[m_fields->curLand]->setRotation(rotation);
						m_fields->landParticleObjects[m_fields->curLand]->setAngle(curAngle);
						m_fields->landParticleObjects[m_fields->curLand]->setGravity(curGravity);
						m_fields->landParticleObjects[m_fields->curLand]->resetSystem();
					}

					m_fields->hasHitGround = true;
				}

				m_fields->groundedElapsed += delta;
				m_fields->sinceGrounded = 0.f;
			}
			else
			{
				m_fields->groundedElapsed = 0.f;
				m_fields->sinceGrounded += delta;
				m_fields->hasHitGround = false;
			}
		}

		PlayerObject::update(delta);

		if (!tlp::settings.isModEnabled || !::isPlayerEnabled(m_isSecondPlayer))
		{
			return;
		}

		float scale = tlp::settings.isLegacyScaling ? 1.f : m_vehicleSize;
		bool isGroundWall = ::isGroundWall(m_fields->ground);

		::updateValues(m_playerGroundParticles, m_fields->dragParticle, scale, m_isUpsideDown, isGroundWall);
		::updateValues(m_trailingParticles, m_fields->trailParticle, scale, false, isGroundWall);
		::updateValues(m_shipClickParticles, m_fields->shipClickParticle, scale, false, isGroundWall);

		if (tlp::settings.isLegacyScaling && m_fields->lastVehicleSize != m_vehicleSize)
		{
			for (auto system : geode::cocos::CCArrayExt<cocos2d::CCParticleSystem*>(m_particleSystems))
			{
				if (std::ranges::find(m_fields->ids, system->getID()) == m_fields->ids.end())
				{
					system->loadScaledDefaults(1.f);
				}

				system->setScale(m_vehicleSize);
			}

			m_fields->lastVehicleSize = m_vehicleSize;
		}


		if (tlp::settings.isLegacyTracking)
		{
			cocos2d::CCPoint factors = cocos2d::CCPoint(10.f, 13.f);
			cocos2d::CCPoint offsets = cocos2d::CCPoint();
			cocos2d::CCPoint newPos = cocos2d::CCPoint();

			if (!isGroundWall)
			{
				offsets = cocos2d::CCPoint(m_isGoingLeft ? factors.x : -factors.x,
										   m_isUpsideDown ? factors.y : -factors.y);

				newPos = cocos2d::CCPoint(getPosition().x + (offsets.x * m_vehicleSize),
										  getPosition().y + (m_isOnGround3 ? offsets.y : -offsets.y * m_vehicleSize));
			}

			if (isGroundWall)
			{
				offsets = cocos2d::CCPoint(m_isGoingLeft ? factors.y : -factors.y,
										   m_isUpsideDown ? factors.x : -factors.x);

				newPos = cocos2d::CCPoint(getPosition().x + (offsets.y * m_vehicleSize),
										  getPosition().y + (m_isOnGround3 ? offsets.x : -offsets.x * m_vehicleSize));
			}
			
			bool isCube = !m_isShip && !m_isBall && !m_isBird && !m_isDart && !m_isRobot && !m_isSpider && !m_isSwing;
			bool isValidGameMode = isCube || m_isBall || m_isRobot || m_isSpider;
			if (m_fields->playedSpiderDashEffect &&
				m_fields->groundedElapsed >= 0.5f &&
				isValidGameMode && isVisible())
			{
				m_playerGroundParticles->resumeSystem();
				m_fields->playedSpiderDashEffect = false;
			}

			m_playerGroundParticles->setPosition(newPos);
		}

		m_dashParticles->setPosition(getPosition());

		m_fields->isOnGround = false;
	}

	void togglePlayerScale(bool isMini, bool p1)
	{
		PlayerObject::togglePlayerScale(isMini, p1);

		if (tlp::settings.isModEnabled && isMini)
		{
			if (!::isPlayerEnabled(m_isSecondPlayer) || tlp::settings.isLegacyScaling)
			{
				return;
			}
			
			m_fields->dragParticle.posVar = m_fields->orgPosVar;
			::copyValues(m_fields->trailParticle, m_fields->dragParticle);
			::copyValues(m_fields->shipClickParticle, m_fields->dragParticle);
		}
	}

	void toggleRobotMode(bool isRobot, bool p1)
	{
		PlayerObject::toggleRobotMode(isRobot, p1);
		::widenedPosVar(m_fields->dragParticle, isRobot, m_isSecondPlayer);
	}

	void toggleSpiderMode(bool isSpider, bool p1)
	{
		PlayerObject::toggleSpiderMode(isSpider, p1);
		::widenedPosVar(m_fields->dragParticle, isSpider, m_isSecondPlayer);
	}

	void hitGround(GameObject* object, bool isCeiling)
	{
		PlayerObject::hitGround(object, isCeiling);

		if (tlp::settings.isModEnabled && !isCeiling)
		{
			if (!::isPlayerEnabled(m_isSecondPlayer))
			{
				return;
			}

			m_fields->isOnGround = true;
		}
	}

	void flipGravity(bool flip, bool noEffects)
	{
		PlayerObject::flipGravity(flip, noEffects);

		if (tlp::settings.isModEnabled && !tlp::settings.isLegacyValues)
		{
			if (!::isPlayerEnabled(m_isSecondPlayer))
			{
				return;
			}

			m_fields->dragParticle.posVar = cocos2d::CCPoint(m_fields->orgPosVar.y, m_fields->orgPosVar.x);
			::copyValues(m_fields->trailParticle, m_fields->dragParticle);
			::copyValues(m_fields->shipClickParticle, m_fields->dragParticle);
		}
	}

	void rotateGameplay(int moveDirection, int groundDirection, bool editVelocity, float velocityModX, float velocityModY, bool overrideVelocity, bool dontSlide)
	{
		PlayerObject::rotateGameplay(moveDirection, groundDirection, editVelocity, velocityModX, velocityModY, overrideVelocity, dontSlide);

		if (tlp::settings.isModEnabled)
		{
			if (!::isPlayerEnabled(m_isSecondPlayer))
			{
				return;
			}

			m_fields->ground = moveDirection - 1u;

			if (m_fields->lastGround != m_fields->ground)
			{
				if (::isGroundWall(m_fields->ground) && !m_fields->valuesShifted)
				{
					::rotateValues(m_fields->dragParticle);
					::rotateValues(m_fields->trailParticle);
					::rotateValues(m_fields->shipClickParticle);
					m_fields->valuesShifted = true;
				}

				if (!::isGroundWall(m_fields->ground) && m_fields->valuesShifted)
				{
					::rotateValues(m_fields->dragParticle);
					::rotateValues(m_fields->trailParticle);
					::rotateValues(m_fields->shipClickParticle);
					m_fields->valuesShifted = false;
				}

				m_fields->lastGround = m_fields->ground;
			}
		}
	}

	void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to)
	{
		PlayerObject::playSpiderDashEffect(from, to);

		if (tlp::settings.isModEnabled && tlp::settings.isLegacyTracking)
		{
			if (!::isPlayerEnabled(m_isSecondPlayer))
			{
				return;
			}

			m_playerGroundParticles->resetSystem();
			m_playerGroundParticles->stopSystem();

			m_fields->playedSpiderDashEffect = true;
		}
	}
};