#pragma once
#include <Geode/modify/PlayerObject.hpp>

namespace gd {

using PlayerObject = ::PlayerObject;

}

namespace tlp {

using namespace geode::prelude;
struct PlayerObject;

} // namespace tlp

struct tlp::PlayerObject : Modify<tlp::PlayerObject, gd::PlayerObject> {
    struct Fields {
        float m_lastVehicleSize = 1.f;
        bool m_wasUpsideDown    = false;
    };

    using Self = ModifyBase<ModifyDerive<tlp::PlayerObject, gd::PlayerObject>>;
    static void onModify(Self&);

    void updatePlayerArt() $override;
    void togglePlayerScale(bool, bool) $override;

    // @note True Legacy Particles Addition
    void onSizeChange();

    // @note True Legacy Particles Addition
    void updateParticles();
};