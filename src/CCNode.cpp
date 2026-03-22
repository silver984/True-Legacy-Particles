#include <tlp/CCNode.hpp>
#include <tlp/Settings.hpp>

void TLPCCNode::setPosition(const cocos2d::CCPoint& position)
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCNode::setPosition(position);
		return;
	}

	if (getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr)
	{
		return;
	}

	cocos2d::CCNode::setPosition(position);
}

void TLPCCNode::setRotation(float fRotation)
{
	auto& settings = tlp::Settings::get();

	if (!settings.isModEnabled())
	{
		cocos2d::CCNode::setRotation(fRotation);
		return;
	}

	if (getID() == "land-particles-0"_spr || getID() == "land-particles-1"_spr)
	{
		return;
	}

	cocos2d::CCNode::setRotation(fRotation);
}