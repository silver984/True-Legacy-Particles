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

    $override void updatePlayerArt();
    $override void togglePlayerScale(bool, bool);

    // @note True Legacy Particles Addition
    void onSizeChange();

    // @note True Legacy Particles Addition
    void updateGroundParticles();
};