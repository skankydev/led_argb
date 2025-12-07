#include "CmdManager.h"
#include <ArduinoJson.h>

CmdManager *CmdManager::instance = nullptr;

CmdManager *CmdManager::getInstance()
{
	if (!instance) {
		instance = new CmdManager;
	}
	return instance;
}

CmdManager::CmdManager() {
	_leds = LedManager::getInstance(); 
}

void CmdManager::newCmd(String message){

	info(message);
	JsonDocument cmd;
	deserializeJson(cmd, message);
	if(cmd["color"].is<JsonVariant>()){
		String colorStr = cmd["color"].as<String>();
		uint32_t color = strtol(colorStr.c_str(), NULL, 16);
		_leds->setColor(color);
	}

	if(cmd["effect"].is<JsonVariant>()){
		unsigned int effect = cmd["effect"].as<unsigned int>();
		_leds->setEffect(effect);
	}

}