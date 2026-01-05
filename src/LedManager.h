#pragma once

#include "setting.h"
#include <WS2812FX.h>
#include <ArduinoJson.h>

class LedManager{

	private:
		static LedManager* instance;
		LedManager();


		WS2812FX _ligne0;

		JsonDocument _scenario;

		uint64_t _timerLigne0 = 0;
		uint8_t _keyLine0 = 0;
		uint64_t _lastChangeLine0 = 0;

		bool _scenarioMode = false;

		unsigned long _lastChange;
		uint32_t _autoDelay;


	public:
		static LedManager* getInstance();

		void step();
		void print();
		void printMode();

		bool initScenario();
		void newScenario(JsonObject scenario);

		void setDefault();
		void setNextEffect(String target = "all");
		void setEffect(uint8_t mode, String target = "all");

		void setColor(uint32_t color, String target = "all");
		void setSpeed(uint16_t speed, String target = "all");
		void setBrightness(uint8_t brightness, String target = "all");
		uint8_t getBrightness( String target = "all");

		void setSegments(JsonArray segments, String target = "all");

		void demo();

		void noel();
		void noelBis();

		void run();
		void blink();
		void rainbow();
		void scanner();
		void theaterChaseRainbow();
		void custome();

		
};
