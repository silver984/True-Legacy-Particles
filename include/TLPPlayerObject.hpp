#pragma once
#include <Geode/modify/PlayerObject.hpp>
struct TLPPlayerObject final : geode::Modify<TLPPlayerObject, PlayerObject> {
    struct Fields {
        Fields();
        cocos2d::CCParticleSystemQuad* m_groundParticles;
        cocos2d::CCParticleSystemQuad* m_trailParticles;
        float m_lastVehicleSize;
        bool m_wasUpsideDown;
    };

    using Self = geode::modifier::ModifyBase<
        geode::modifier::ModifyDerive<TLPPlayerObject, PlayerObject>>;
    static void onModify(Self& self);

    bool init(int player, int ship, GJBaseGameLayer* gameLayer,
              cocos2d::CCLayer* layer, bool playLayer);

    // tlp addition
    void initGroundParticles();

    // tlp addition
    void initTrailParticles();

    void addAllParticles();
    void stopParticles();

    void flipGravity(bool flip, bool noEffects);

    void togglePlayerScale(bool enable, bool noEffects);
    void toggleRobotMode(bool enable, bool noEffects);
    void toggleSpiderMode(bool enable, bool noEffects);

    // tlp addition
    void onSizeChange(float size);

    // tlp addition
    void onGravityFlip();

    // tlp addition
    void updateParticles();

    // tlp addition
    void updateParticleColors();

    // tlp addition
    void updateGroundParticlesPos();

    // tlp addition
    void updateGroundParticlesEmission();
};