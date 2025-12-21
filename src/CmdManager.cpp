#include "CmdManager.h"
#include <ArduinoJson.h>
#include "MyMqtt.h"


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

	JsonDocument cmd;
	deserializeJson(cmd, message);
	if(cmd["timestamp"].is<JsonVariant>()){
		setTime(cmd["timestamp"].as<uint32_t>());
		println(vert("Time is set") +" : "+getDateTime(),"CMD" );
	}

	if(cmd["color"].is<JsonVariant>()){
		String colorStr = cmd["color"].as<String>();
		if(colorStr.startsWith("#")){
			colorStr = colorStr.substring(1);
		}
		uint32_t color = strtol(colorStr.c_str(), NULL, 16);
		_leds->setColor(color);
	}

	if(cmd["effect"].is<JsonVariant>()){
		unsigned int effect = cmd["effect"].as<unsigned int>();
		_leds->setEffect(effect);
	}

	if(cmd["segments"].is<JsonVariant>()){
		_leds->setSegments(cmd["segments"].as<JsonArray>());
	}

	if(cmd["update"].is<JsonVariant>()){
		WiFiTool::getInstance()->update(cmd["update"].is<Sring>());
	}
}

void CmdManager::sayHello(){

	JsonDocument cmd;
	cmd["cmd"] = "hello";
	cmd["data"]["version"] = VERSION;

	String message;
	serializeJson(cmd,message);
	MyMqtt::getInstance()->send(message);
}
