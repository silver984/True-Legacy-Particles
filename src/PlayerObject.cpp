#include <Geode/binding/GJBaseGameLayer.hpp>
#include <Geode/modify/CCParticleSystem.hpp>

#include <tlp/PlayerObject.hpp>
#include <tlp/Settings.hpp>

using namespace geode::prelude;

namespace {

bool isLegacyScaling = false;

}

$on_mod(Loaded) {
    tlp::settings::listener<bool>("2.1-scaling", [](bool val) -> void { isLegacyScaling = val; });
}

namespace tlp {

struct CCParticleSystem;

} // namespace tlp

struct tlp::CCParticleSystem : Modify<tlp::CCParticleSystem, cocos2d::CCParticleSystem> {
    struct Fields {
        cocos2d::CCPoint m_fDefaultModeAGravity;
    };

    using Self = ModifyBase<ModifyDerive<tlp::CCParticleSystem, cocos2d::CCParticleSystem>>;
    static void onModify(Self& self) {
        (void)self.setHookPriority("cocos2d::CCParticleSystem::loadScaledDefaults", Priority::Last);
    }

    bool initWithFile(char const* plistFile, bool unk) $override {
        if (!cocos2d::CCParticleSystem::initWithFile(plistFile, unk)) {
            return false;
        }
        m_fields->m_fDefaultModeAGravity = getGravity();
        return true;
    }

    void loadScaledDefaults(float) $override {
        // do nothing
    }

    // @note True Legacy Particles Addition
    void loadScaledDefaults2(float scale) {
        setStartSize(m_fDefaultStartSize * scale);
        setStartSizeVar(m_fDefaultStartSizeVar * scale);
        setEndSize(m_fDefaultEndSize * scale);
        setAngle(m_fDefaultAngle * scale);
        setSpeed(m_fDefaultModeASpeed * scale);
        setSpeedVar(m_fDefaultModeASpeedVar * scale);
        setPosVar(m_tDefaultPosVar * scale);
    }

    // @note True Legacy Particles Addition
    void toggleFlipGravityAndAngle(bool val) {
        setGravity(val ? -m_fields->m_fDefaultModeAGravity : m_fields->m_fDefaultModeAGravity);
        setAngle(val ? -m_fDefaultAngle : m_fDefaultAngle);
    }
};

void tlp::PlayerObject::onModify(Self& self) {
    (void)self.setHookPriority("PlayerObject::updatePlayerArt", Priority::Last);
}

void tlp::PlayerObject::updatePlayerArt() {
    if (m_isShip || m_isBird || m_isDart || m_isRobot || m_isSpider) {
        m_mainLayer->setScaleY(m_isUpsideDown ? -1.f : 1.f);
    } else {
        m_mainLayer->setScaleY(1.f);
    }

    m_mainLayer->setScaleX(m_isGoingLeft ? -1.f : 1.f);

    if (m_isRobot || m_isSpider) {
        m_playerGroundParticles->setPosVar(cocos2d::CCPoint(15, 0));
    }

    modify_cast<tlp::CCParticleSystem*>(m_playerGroundParticles)
        ->toggleFlipGravityAndAngle(m_isUpsideDown);
}

void tlp::PlayerObject::togglePlayerScale(bool enable, bool noEffects) {
    gd::PlayerObject::togglePlayerScale(enable, noEffects);
    if (m_fields->m_lastVehicleSize != m_vehicleSize) {
        m_fields->m_lastVehicleSize = m_vehicleSize;
        onSizeChange();
    }
}

void tlp::PlayerObject::onSizeChange() {
    for (cocos2d::CCObject* obj : CCArrayExt(m_particleSystems)) {
#ifdef GEODE_IS_WINDOWS
        auto* constituent = typeinfo_cast<cocos2d::CCParticleSystemQuad*>(obj);
#else
        auto* constituent = dynamic_cast<cocos2d::CCParticleSystemQuad*>(obj);
#endif

        if (!constituent) {
            continue;
        }

        auto* particle = modify_cast<tlp::CCParticleSystem*>(constituent);

        if (isLegacyScaling) {
            particle->loadScaledDefaults2(m_vehicleSize);
        } else {
            particle->setScale(m_vehicleSize);
        }
    }
}

void tlp::PlayerObject::updateParticles() {
    constexpr float X_OFFSET = 10.f;
    constexpr float Y_OFFSET = 13.f;

    cocos2d::CCPoint factor(m_isGoingLeft ? -X_OFFSET : X_OFFSET,
                            m_isUpsideDown ? -Y_OFFSET : Y_OFFSET);

    if (m_isRobot || m_isSpider) {
        constexpr float Y_FACTOR = 2.f;
        factor.y += m_isUpsideDown ? -Y_FACTOR : Y_FACTOR;
    }

    if (m_isBall && !m_isOnGround3) {
        factor.y = -factor.y;
    }

    factor *= m_vehicleSize;
    cocos2d::CCPoint position = this->getPosition();

    m_playerGroundParticles->setPosition(position - factor);

    if (m_isDashing) {
        m_dashParticles->setPosition(position);
    }
}