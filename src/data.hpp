#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Data {
public:
	static Data& get() {
		static Data instance;
		return instance;
	}

	bool isInLevel;
	float deltaTime = 1.f / 60;
	bool legacyTracking;
	bool legacyScaling;
	bool inconstVal;
};