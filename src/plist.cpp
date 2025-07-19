#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "globals/settings.hpp"
#include "globals/particle.hpp"
#include "util.hpp"

using namespace geode::prelude;

class $modify(PlayerObject) {
	bool init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer) {
		if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
			return false;

		CCDictionary* dragDict = CCDictionary::createWithContentsOfFile("dragEffect.plist");
		CCDictionary* landDict = CCDictionary::createWithContentsOfFile("landEffect.plist");

		g_sourceGravity = CCPoint(
			((CCString*)dragDict->objectForKey("gravityx"))->floatValue(),
			((CCString*)dragDict->objectForKey("gravityy"))->floatValue()
		);

		g_landGravity = CCPoint(
			((CCString*)landDict->objectForKey("gravityx"))->floatValue(),
			((CCString*)landDict->objectForKey("gravityy"))->floatValue()
		);
		g_landAngle = ((CCString*)landDict->objectForKey("angle"))->floatValue();

		// y for variance
		g_sourceAngle = CCPoint(
			((CCString*)dragDict->objectForKey("angle"))->floatValue(),
			((CCString*)dragDict->objectForKey("angleVariance"))->floatValue()
		);

		// y for variance
		g_sourceStartSize = CCPoint(
			((CCString*)dragDict->objectForKey("startParticleSize"))->floatValue(),
			((CCString*)dragDict->objectForKey("startParticleSizeVariance"))->floatValue()
		);

		// y for variance
		g_sourceSpeed = CCPoint(
			((CCString*)dragDict->objectForKey("speed"))->floatValue(),
			((CCString*)dragDict->objectForKey("speedVariance"))->floatValue()
		);

		g_sourcePosVar = CCPoint(
			((CCString*)dragDict->objectForKey("sourcePositionVariancex"))->floatValue(),
			((CCString*)dragDict->objectForKey("sourcePositionVariancey"))->floatValue()
		);

		for (int i = 0; i < 2; i++) {
			g_drag[i].posVar = g_sourcePosVar;
			g_drag[i].speed = g_sourceSpeed.x;
			g_drag[i].speedVar = g_sourceSpeed.y;
			g_drag[i].startSize = g_sourceStartSize.x;
			g_drag[i].startSizeVar = g_sourceStartSize.y;

			g_trail[i].posVar = CCPoint(0, g_sourcePosVar.y * 0.4);
			g_trail[i].speed = g_sourceSpeed.x * 0.2;
			g_trail[i].speedVar = g_sourceSpeed.y * 0.2;

			g_shipClick[i].posVar = CCPoint(0, g_sourcePosVar.y * 0.4);
			g_shipClick[i].speed = g_sourceSpeed.x * 2;
			g_shipClick[i].speedVar = g_sourceSpeed.y * 2;
			g_shipClick[i].startSize = g_sourceStartSize.x * 1.5;
			g_shipClick[i].startSizeVar = g_sourceStartSize.y * 1.5;

			g_particle[i].isGrounded = false;
			g_particle[i].isDeviated = false;
			g_land[i].gate = false;
			g_land[i].landSwitch = 0;
		}

		return true;
	}
};