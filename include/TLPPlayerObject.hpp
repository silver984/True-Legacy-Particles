#pragma once
#include <Geode/modify/PlayerObject.hpp>
struct TLPPlayerObject final : geode::Modify<TLPPlayerObject, PlayerObject> {
    struct Fields {
        Fields();
        float m_lastVehicleSize;
        bool m_wasUpsideDown;
    };

    using Self = geode::modifier::ModifyBase<
        geode::modifier::ModifyDerive<TLPPlayerObject, PlayerObject>>;
    static void onModify(Self&);

    void updatePlayerArt();
    void togglePlayerScale(bool, bool);
    void flipGravity(bool, bool);

    // @note True Legacy Particles Addition
    void onSizeChange();

    // @note True Legacy Particles Addition
    void onGravityFlip();

    // @note True Legacy Particles Addition
    void updateGroundParticles();
};