#pragma once
#include <Geode/modify/GJBaseGameLayer.hpp>
struct TLPGJBaseGameLayer final
    : geode::Modify<TLPGJBaseGameLayer, GJBaseGameLayer> {
    void update(float dt) override;

    void createPlayer();
};