#pragma once
#include <Geode/modify/CCNode.hpp>

struct TLPCCNode : geode::Modify<TLPCCNode, cocos2d::CCNode> {
	void setPosition(const cocos2d::CCPoint& position) override;
	void setRotation(float fRotation) override;
};