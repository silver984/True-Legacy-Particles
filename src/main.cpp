#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "functions/Core.hpp"
#include "functions/AlwaysSpawn.hpp"
#include "functions/util.hpp"
#include "functions/variance.hpp"
#include "variables/particle.hpp"
#include "variables/settings.hpp"

using namespace geode::prelude;

bool warningPassed = false;

class $modify(PlayLayer) {
	void startGame() {
		if (!toggle)
			return PlayLayer::startGame();

#ifdef GEODE_IS_WINDOWS
		bool isQOLLoaded = Loader::get()->isModLoaded("thesillydoggo.qolmod");
		bool showWarning = Mod::get()->getSettingValue<bool>("warning");

		if (isQOLLoaded && !showWarning && !warningPassed) {
			auto winSize = CCDirector::get()->getWinSize();
			float fadeInTime = 0.25f;
			float uptime = 3.f;

			// Reusable fade sequence generator
			auto makeFadeSequence = [=]() {
				return CCSequence::create(
					CCFadeIn::create(fadeInTime),
					CCDelayTime::create(uptime),
					CCFadeOut::create(fadeInTime),
					nullptr
				);
				};

			// gradient background
			auto gradient = CCLayerGradient::create(ccc4(0, 0, 0, 0), ccc4(0, 0, 0, 128));
			gradient->setContentSize({ winSize.width, winSize.height / 4 });
			gradient->setAnchorPoint({ 0, 0 });
			gradient->setPosition({ 0, 0 });
			gradient->setID("tlp-shadow-gradient");
			this->addChild(gradient, 9998);
			gradient->runAction(makeFadeSequence());

			// logo
			auto logo = CCSprite::create("logo.png"_spr);
			logo->setPosition({ 30, 30 });
			logo->setScale(0.4f);
			logo->setID("tlp-logo");
			this->addChild(logo, 9999);
			logo->runAction(makeFadeSequence());

			// "True Legacy Particles" text
			std::string modName = Mod::get()->getName();
			auto text = CCLabelBMFont::create(modName.c_str(), "bigFont.fnt");
			text->setScale(0.3f);
			text->setAnchorPoint({ 0, 1 });

			CCPoint textPos{
				logo->getPositionX() + (logo->getContentWidth() * logo->getScale()) / 2 + 5,
				logo->getPositionY() + (logo->getContentHeight() * logo->getScale()) / 2 - 5
			};
			text->setPosition(textPos);
			text->setID("tlp-text");
			this->addChild(text, 9999);
			text->runAction(makeFadeSequence());

			// exclamation mark
			auto exclamation = CCLabelBMFont::create("!", "bigFont.fnt");
			exclamation->setScale(0.25f);
			exclamation->setAnchorPoint({ 0, 1 });

			CCPoint excPos{
				text->getPositionX(),
				text->getPositionY() - (text->getContentHeight() * text->getScale()) - 5
			};
			exclamation->setPosition(excPos);
			exclamation->setColor({ 255, 0, 0 });
			exclamation->setID("tlp-exclamation");
			this->addChild(exclamation, 9999);
			exclamation->runAction(makeFadeSequence());

			// compatibility warning
			std::string incompatibleModName = Loader::get()->getInstalledMod("thesillydoggo.qolmod")->getName();
			auto warning = CCLabelBMFont::create(fmt::format("May not work with {}", incompatibleModName).c_str(), "bigFont.fnt");
			warning->setScale(0.25f);
			warning->setAnchorPoint({ 0, 1 });

			CCPoint warningPos{
				exclamation->getPositionX() + exclamation->getContentHeight() * exclamation->getScale(),
				exclamation->getPositionY()
			};
			warning->setPosition(warningPos);
			warning->setID("tlp-warning");
			this->addChild(warning, 9999);
			warning->runAction(makeFadeSequence());

			// bottom hint
			auto bottomText = CCLabelBMFont::create("DISABLE THIS WARNING IN MOD SETTINGS", "bigFont.fnt");
			bottomText->setScale(0.15f);
			bottomText->setPosition({ winSize.width / 2, 6 });
			bottomText->setOpacity(0);
			bottomText->setID("tlp-bottom-text");
			this->addChild(bottomText, 9999);
			bottomText->runAction(CCSequence::create(
				CCFadeTo::create(fadeInTime, 128),
				CCDelayTime::create(uptime),
				CCFadeTo::create(fadeInTime, 0),
				nullptr
			));

			warningPassed = true;
		}
#endif

		PlayLayer::startGame();
	}
};

class $modify(PlayerObject) {
	bool init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer) {
		if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
			return false;

		toggle = Mod::get()->getSettingValue<bool>("switch");
		legacyTracking = Mod::get()->getSettingValue<bool>("legacy-tracking");
		legacyScaling = Mod::get()->getSettingValue<bool>("legacy-scaling");
		isTrueVals = Mod::get()->getSettingValue<bool>("true-vals");
		alwaysSpawn = Mod::get()->getSettingValue<bool>("always-spawn");
		noRotation = Mod::get()->getSettingValue<bool>("no-rotation");
		centerDash = Mod::get()->getSettingValue<bool>("center-dash");
		player1 = Mod::get()->getSettingValue<bool>("player-1");
		player2 = Mod::get()->getSettingValue<bool>("player-2");

		if (centerDash) {
			centerDashX = Mod::get()->getSettingValue<bool>("center-dash-x");
			centerDashY = Mod::get()->getSettingValue<bool>("center-dash-y");
		}
		else {
			centerDashX = false;
			centerDashY = false;
		}

		for (int i = 0; i < 2; i++) {
			CCDictionary* dragDict =
				CCDictionary::createWithContentsOfFile("dragEffect.plist");

			CCDictionary* landDict =
				CCDictionary::createWithContentsOfFile("landEffect.plist");

			sourceGravity = CCPoint(
				((CCString*)dragDict->objectForKey("gravityx"))->floatValue(),
				((CCString*)dragDict->objectForKey("gravityy"))->floatValue()
			);

			landGravity = CCPoint(
				((CCString*)landDict->objectForKey("gravityx"))->floatValue(),
				((CCString*)landDict->objectForKey("gravityy"))->floatValue()
			);
			landAngle = ((CCString*)landDict->objectForKey("angle"))->floatValue();

			// y for variance
			sourceAngle = CCPoint(
				((CCString*)dragDict->objectForKey("angle"))->floatValue(),
				((CCString*)dragDict->objectForKey("angleVariance"))->floatValue()
			);

			// y for variance
			sourceStartSize = CCPoint(
				((CCString*)dragDict->objectForKey("startParticleSize"))->floatValue(),
				((CCString*)dragDict->objectForKey("startParticleSizeVariance"))->floatValue()
			);

			// y for variance
			sourceSpeed = CCPoint(
				((CCString*)dragDict->objectForKey("speed"))->floatValue(),
				((CCString*)dragDict->objectForKey("speedVariance"))->floatValue()
			);

			sourcePosVar = CCPoint(
				((CCString*)dragDict->objectForKey("sourcePositionVariancex"))->floatValue(),
				((CCString*)dragDict->objectForKey("sourcePositionVariancey"))->floatValue()
			);

			drag[i].posVar = sourcePosVar;
			drag[i].speed = sourceSpeed.x;
			drag[i].speedVar = sourceSpeed.y;
			drag[i].startSize = sourceStartSize.x;
			drag[i].startSizeVar = sourceStartSize.y;

			trail[i].posVar = CCPoint(0, sourcePosVar.y * 0.4);
			trail[i].speed = sourceSpeed.x * 0.2;
			trail[i].speedVar = sourceSpeed.y * 0.2;

			shipClick[i].posVar = CCPoint(0, sourcePosVar.y * 0.4);
			shipClick[i].speed = sourceSpeed.x * 2;
			shipClick[i].speedVar = sourceSpeed.y * 2;
			shipClick[i].startSize = sourceStartSize.x * 1.5;
			shipClick[i].startSizeVar = sourceStartSize.y * 1.5;

			particle[i].isGrounded = false;
			gate[i] = 0;
			landSwitch[i] = 0;
		}

		return true;
	}

	virtual void resetObject() {
		m_landParticles0->setRotation(0);
		m_landParticles1->setRotation(0);

		for (int i = 0; i < 2; i++) {
			particle[i].isGrounded = false;
			gate[i] = 0;
			landSwitch[i] = 0;
		}

		PlayerObject::resetObject();
	}

	void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to) {
		particle[index(this)].spiderDashed = true;
		PlayerObject::playSpiderDashEffect(from, to);
	}

	void togglePlayerScale(bool isMini, bool p1) {
		scale(this, isMini);
		PlayerObject::togglePlayerScale(isMini, p1);
	}

	void toggleRobotMode(bool isRobot, bool p1) {
		wide(this, isRobot);
		PlayerObject::toggleRobotMode(isRobot, p1);
	}

	void toggleSpiderMode(bool isSpider, bool p1) {
		wide(this, isSpider);
		PlayerObject::toggleSpiderMode(isSpider, p1);
	}

	void hitGround(GameObject* p0, bool p1) {
		if (!p1)
			particle[index(this)].isGrounded = true;

		PlayerObject::hitGround(p0, p1);
	}

	virtual void update(float p0) {
		if (!check(this, true))
			return PlayerObject::update(p0);

		core(this);

		if (noRotation) {
			int angle = this->m_isSideways ? -90 : 0;
			m_landParticles0->setRotation(angle);
			m_landParticles1->setRotation(angle);
		}

		PlayerObject::update(p0);

		particle[index(this)].isGrounded = false;

		if (centerDashX)
			m_dashParticles->setPositionX(this->getPositionX());
		if (centerDashY)
			m_dashParticles->setPositionY(this->getPositionY());
	}
};