#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/PlayerObject.hpp>

$on_mod(Loaded) {
    auto toggleHooks = [](bool val) -> void {
        for (auto& hook : geode::Mod::get()->getHooks())
            (void) hook->toggle(val);
    };
    constexpr char const* settingName = "switch";
    toggleHooks(geode::Mod::get()->getSettingValue<bool>(settingName));
    geode::listenForSettingChanges<bool>(std::string(settingName), toggleHooks);
}

struct TLPPlayerObject : geode::Modify<TLPPlayerObject, PlayerObject> {
    struct Fields {
        Fields()
            : m_groundParticles(nullptr), m_lastVehicleSize(1.f),
              m_wasUpsideDown(false) {}
        cocos2d::CCParticleSystemQuad* m_groundParticles;
        float m_lastVehicleSize;
        bool m_wasUpsideDown;
    };

    bool init(int player, int ship, GJBaseGameLayer* gameLayer,
              cocos2d::CCLayer* layer, bool playLayer) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
            return false;

        auto& groundParticles = m_fields->m_groundParticles;
        groundParticles =
            cocos2d::CCParticleSystemQuad::create("dragEffect.plist", false);
        using enum cocos2d::tCCPositionType;
        groundParticles->setPositionType(kCCPositionTypeRelative);
        groundParticles->setID("ground-particles"_spr);
        // remove `m_playerGroundParticles` from `m_particleSystems` and replace
        // it with our `m_fields->m_groundParticles`
        m_particleSystems->removeObject(m_playerGroundParticles, false);
        m_particleSystems->addObject(groundParticles);

        return true;
    }

    void addAllParticles() {
        PlayerObject::addAllParticles();
        m_parentLayer->addChild(m_fields->m_groundParticles);
        // remove the original ground particles from its parent, assuming that
        // it has already been added by PlayerObject::addAllParticles
        m_playerGroundParticles->removeFromParent();
    }

    void stopParticles() {
        PlayerObject::stopParticles();
        m_fields->m_groundParticles->stopSystem();
    }

    void update(float dt) override {
        PlayerObject::update(dt);
        updateGroundParticlesPos();
        updateGroundParticlesEmission();
    }

    void togglePlayerScale(bool enable, bool noEffects) {
        PlayerObject::togglePlayerScale(enable, noEffects);
        if (m_fields->m_lastVehicleSize != m_vehicleSize) {
            onSizeChange(m_vehicleSize);
            m_fields->m_lastVehicleSize = m_vehicleSize;
        }
    }

    void flipGravity(bool flip, bool noEffects) {
        PlayerObject::flipGravity(flip, noEffects);
        if (m_fields->m_wasUpsideDown != m_isUpsideDown) {
            onGravityChange();
            m_fields->m_wasUpsideDown = m_isUpsideDown;
        }
    }

    // both the robot and spider gamemodes change the position variance of the
    // ground particles to this value: (x: 15, y: 0). both of these hooks are
    // similar enough so i just made a macro to reduce repitition
#define GEODE_TOGGLE_SURFACE_GAMEMODE(name)                                    \
    void toggle##name##Mode(bool enable, bool noEffects) {                     \
        PlayerObject::toggle##name##Mode(enable, noEffects);                   \
        if (enable)                                                            \
            m_fields->m_groundParticles->setPosVar(ccp(15.f, 0.f));            \
    }
    GEODE_TOGGLE_SURFACE_GAMEMODE(Robot)
    GEODE_TOGGLE_SURFACE_GAMEMODE(Spider)
#undef GEODE_TOGGLE_SURFACE_GAMEMODE

    // tlp addition
    void onSizeChange(float size) {
        m_fields->m_groundParticles->loadScaledDefaults(size);
    }

    // tlp addition
    void onGravityChange() {
        auto& groundParticles                = m_fields->m_groundParticles;
        cocos2d::CCPoint groundParticlesGrav = groundParticles->getGravity();
        groundParticlesGrav.y                = -groundParticlesGrav.y;
        groundParticles->setGravity(groundParticlesGrav);
        groundParticles->setAngle(-groundParticles->getAngle());
    }

    // tlp addition
    void updateGroundParticlesPos() {
        cocos2d::CCPoint factor(10.f, m_isUpsideDown ? -13.f : 13.f);
        if (m_isBall && !m_isOnGround3)
            factor.y = -factor.y;
        factor *= m_vehicleSize;
        m_fields->m_groundParticles->setPosition(this->getPosition() - factor);
    }

    // tlp addition
    void updateGroundParticlesEmission() {
        auto& obj = m_fields->m_groundParticles;
        if (m_hasGroundParticles && this->isVisible()) {
            if (!obj->isActive())
                obj->resumeSystem();
            return;
        }
        if (obj->isActive())
            obj->stopSystem();
    }
};
