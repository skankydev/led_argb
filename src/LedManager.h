#pragma once

#include "setting.h"
#include "LedConfig.h"
#include <map>
#include <ArduinoJson.h>

enum AppMode {
	MODE_SCENARIO,
	MODE_LIVE
};

class LedManager {
	private:
		static LedManager* instance;
		LedManager();

		std::map<String, LedConfig*> _lines;

		JsonDocument _scenario;
		bool _scenarioMode = false;

		AppMode _appMode = MODE_SCENARIO;

		unsigned long _lastChange;
		uint32_t _autoDelay;

	public:
		static LedManager* getInstance();

		void step();
		//void print();
		void printMode();

		bool initScenario();
		void newScenario(JsonObject scenario);

		void setAppMode(AppMode mode);
		AppMode getAppMode();

		void setDefault();
		void setNextEffect(String target = "all");
		void setEffect(uint8_t mode, String target = "all");

		void setColor(uint32_t color, String target = "all");
		void setSpeed(uint16_t speed, String target = "all");
		void setBrightness(uint8_t brightness, String target = "all");
		uint8_t getBrightness(String target = "all");

		void setSegments(JsonArray segments, String target = "all");

};