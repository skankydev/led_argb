#include "LedConfig.h"

LedConfig::LedConfig(uint8_t pin, uint16_t numLeds) 
	: _line(numLeds, pin, NEO_GRB + NEO_KHZ800) {
	_timer = 0;
	_key = 0;
	_lastChange = 0;
}

void LedConfig::init() {
	_line.init();
	this->setDefault();
	_line.start();
}

void LedConfig::service() {
	_line.service();
}

bool LedConfig::needsUpdate(unsigned long now) {
	return (now - _lastChange) > _timer;
}

void LedConfig::applyStep(JsonObject step) {
	_timer = step["duration"].as<uint64_t>() * 1000;
	this->setSegments(step["segments"].as<JsonArray>());
	_key++;
}

void LedConfig::resetKey() {
	_key = 0;
}

uint8_t LedConfig::getKey() {
	return _key;
}

void LedConfig::setTimer(uint64_t timer) {
	_timer = timer;
}

void LedConfig::updateLastChange(unsigned long now) {
	_lastChange = now;
}

void LedConfig::setSegments(JsonArray segments) {
	_line.resetSegments();
	_line.resetSegmentRuntimes();

	int id = 0;
	for(JsonVariant segment : segments) {
		uint16_t first = segment["first"].as<uint16_t>();
		uint16_t last = segment["last"].as<uint16_t>();
		uint8_t effect = segment["effect"].as<uint8_t>();
		uint16_t speed = segment["speed"].as<uint16_t>();
		bool reverse = segment["reverse"].as<bool>();
		
		JsonArray colors = segment["colors"].as<JsonArray>();
		uint32_t color[3];
		color[0] = convertColor(colors[0]) | 0;
		color[1] = convertColor(colors[1]) | 0;
		color[2] = convertColor(colors[2]) | 0;

		_line.setSegment(id, first, last, effect, color, speed, reverse);
		id++;
	}
}

void LedConfig::setEffect(uint8_t mode) {
	_line.setMode(mode);
}

void LedConfig::setColor(uint32_t color) {
	_line.setColor(color);
}

void LedConfig::setSpeed(uint16_t speed) {
	_line.setSpeed(speed);
}

void LedConfig::setBrightness(uint8_t brightness) {
	_line.setBrightness(brightness);
}

uint8_t LedConfig::getBrightness() {
	return _line.getBrightness();
}

void LedConfig::setDefault() {
	_line.resetSegments();
	_line.resetSegmentRuntimes();
	_line.setSegment(0, 0, NUM_LEDS-1, FX_MODE_RAINBOW_CYCLE, 0x0000FF, 1500, false);
	_line.setBrightness(50);
}

uint32_t LedConfig::getMode() {
	return _line.getMode();
}

String LedConfig::getModeName(uint8_t mode) {
	return String(_line.getModeName(mode));
}