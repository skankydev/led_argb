#pragma once

#include "setting.h"
#include <WS2812FX.h>
#include <ArduinoJson.h>

class LedManager{

	private:
		static LedManager* instance;
		LedManager();


		WS2812FX _ligne1;

		bool _autoMode = false;
		unsigned long _lastChange;
		uint32_t _autoDelay;


	public:
		static LedManager* getInstance();

		void step();
		void print();
		void printMode();

		void toggleAutoMode();
		bool getAutoMode();
		void setAutoDelay(uint32_t delayMs);

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
