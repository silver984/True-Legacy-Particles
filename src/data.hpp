#pragma once

class Data {
public:
	static Data& get() {
		static Data instance;
		return instance;
	}

	bool modSwitch;
	bool legacyTracking;
	bool legacyScaling;
	bool inconstVal;
	bool noRotation;
	bool centerDash;
	bool centerDashX;
	bool centerDashY;
	bool player1;
	bool player2;
};