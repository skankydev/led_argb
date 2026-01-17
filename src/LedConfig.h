#pragma once

#include "setting.h"
#include <WS2812FX.h>
#include <ArduinoJson.h>

class LedConfig {
	private:
		WS2812FX _line;
		
		uint64_t _timer;
		uint8_t _key;
		uint64_t _lastChange;

	public:
		LedConfig(uint8_t pin, uint16_t numLeds);
		
		void init();
		void service();
		
		// Gestion scénario
		bool needsUpdate(unsigned long now);
		void applyStep(JsonObject step);
		void resetKey();
		uint8_t getKey();
		void setTimer(uint64_t timer);
		void updateLastChange(unsigned long now);
		
		// Contrôles LED
		void setSegments(JsonArray segments);
		void setEffect(uint8_t mode);
		void setColor(uint32_t color);
		void setSpeed(uint16_t speed);
		void setBrightness(uint8_t brightness);
		uint8_t getBrightness();
		
		void setDefault();
		
		// Getters pour debug
		uint32_t getMode();
		String getModeName(uint8_t mode);
};