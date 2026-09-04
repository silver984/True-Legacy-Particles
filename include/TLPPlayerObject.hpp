#pragma once
#include <Geode/modify/PlayerObject.hpp>

struct TLPPlayerObject final : geode::Modify<TLPPlayerObject, PlayerObject> {
    struct Fields {
        Fields();
        float m_lastVehicleSize = 1.f;
        bool m_wasUpsideDown    = false;
    };

    using Self = geode::modifier::ModifyBase<
        geode::modifier::ModifyDerive<TLPPlayerObject, PlayerObject>>;
    static void onModify(Self&);

    void updatePlayerArt() $override;
    void togglePlayerScale(bool, bool) $override;

    // @note True Legacy Particles Addition
    void onSizeChange();

    // @note True Legacy Particles Addition
    void updateGroundParticles();
};