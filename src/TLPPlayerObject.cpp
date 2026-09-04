#include <Geode/binding/GJBaseGameLayer.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include <Settings.hpp>
#include <TLPPlayerObject.hpp>

using namespace geode::cast;

struct TLPCCParticleSystem final
    : geode::Modify<TLPCCParticleSystem, cocos2d::CCParticleSystem> {
    using Self = geode::modifier::ModifyBase<geode::modifier::ModifyDerive<
        TLPCCParticleSystem, cocos2d::CCParticleSystem>>;
    struct Fields {
        cocos2d::CCPoint m_fDefaultModeAGravity;
    };

    static void onModify(Self& self) {
        (void)self.setHookPriority(
            "cocos2d::CCParticleSystem::loadScaledDefaults",
            geode::Priority::Last);
    }

    bool initWithFile(char const* plistFile, bool unk) $override {
        if (!CCParticleSystem::initWithFile(plistFile, unk)) return false;
        m_fields->m_fDefaultModeAGravity = getGravity();
        return true;
    }

    void loadScaledDefaults(float) $override {
        // intentionally do nothing
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
        auto& f = m_fields;
        setGravity(val ? -f->m_fDefaultModeAGravity :
                         f->m_fDefaultModeAGravity);
        setAngle(val ? -m_fDefaultAngle : m_fDefaultAngle);
    }
};

void TLPPlayerObject::onModify(Self& self) {
    (void)self.setHookPriority("PlayerObject::updatePlayerArt",
                               geode::Priority::Last);
}

void TLPPlayerObject::updatePlayerArt() {
    if (m_isShip || m_isBird || m_isDart || m_isRobot || m_isSpider)
        m_mainLayer->setScaleY(m_isUpsideDown ? -1.f : 1.f);
    else
        m_mainLayer->setScaleY(1.f);
    m_mainLayer->setScaleX(m_isGoingLeft ? -1.f : 1.f);
    if (m_isRobot || m_isSpider) m_playerGroundParticles->setPosVar(ccp(15, 0));
    modify_cast<TLPCCParticleSystem*>(m_playerGroundParticles)
        ->toggleFlipGravityAndAngle(m_isUpsideDown);
}

void TLPPlayerObject::togglePlayerScale(bool enable, bool noEffects) {
    PlayerObject::togglePlayerScale(enable, noEffects);
    if (m_fields->m_lastVehicleSize != m_vehicleSize) {
        onSizeChange();
        m_fields->m_lastVehicleSize = m_vehicleSize;
    }
}

void TLPPlayerObject::onSizeChange() {
    for (cocos2d::CCObject* obj : geode::cocos::CCArrayExt(m_particleSystems)) {
        TLPCCParticleSystem* particle = modify_cast<TLPCCParticleSystem*>(
            typeinfo_cast<cocos2d::CCParticleSystemQuad*>(obj));
        if (settings::is2p1ScalingEnabled())
            particle->loadScaledDefaults2(m_vehicleSize);
        else
            particle->setScale(m_vehicleSize);
    }
}

void TLPPlayerObject::updateGroundParticles() {
    constexpr float X_OFFSET = 10.f;
    constexpr float Y_OFFSET = 13.f;
    cocos2d::CCPoint factor(m_isGoingLeft ? -X_OFFSET : X_OFFSET,
                            m_isUpsideDown ? -Y_OFFSET : Y_OFFSET);
    if (m_isRobot || m_isSpider) factor.y += 2.f;
    if (m_isBall && !m_isOnGround3) factor.y = -factor.y;
    factor *= m_vehicleSize;
    m_playerGroundParticles->setPosition(this->getPosition() - factor);
}