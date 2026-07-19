#pragma once
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>

struct TLPGJBaseGameLayer final
    : geode::Modify<TLPGJBaseGameLayer, GJBaseGameLayer> {
    void update(float dt) override;
    void createPlayer();
};

struct TLPMenuGameLayer final : geode::Modify<TLPMenuGameLayer, MenuGameLayer> {
    void update(float dt) override;
    void resetPlayer();
};