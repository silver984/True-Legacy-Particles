#include <tlp/CCNode.hpp>

void TLPCCNode::setPosition(const cocos2d::CCPoint& position)
{
	if (getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr)
	{
		return;
	}

	cocos2d::CCNode::setPosition(position);
}

void TLPCCNode::setRotation(float fRotation)
{
	if (getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr)
	{
		return;
	}

	cocos2d::CCNode::setRotation(fRotation);
}