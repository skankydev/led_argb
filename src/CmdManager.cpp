#include "CmdManager.h"
#include <ArduinoJson.h>
#include "MyMqtt.h"
#include "WiFiTool.h"


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
		WiFiTool::getInstance()->update(cmd["update"].as<String>());
	}

	/*if(cmd["scenario"].is<JsonVariant>()){
		_leds->newScenario(cmd["scenario"].as<JsonObject>());
	}*/

	if (cmd["scenario_start"].is<JsonVariant>()) {
        JsonObject start = cmd["scenario_start"];
        _scenarioId = start["id"].as<String>();
        _targetChunks = start["total_chunks"];
        _receivedChunks = 0;
        _buffer = "";
        
        println(jaune("Receiving scenario: ") + _scenarioId, "CMD");
        println("Expected chunks: " + String(_targetChunks), "CMD");
    }
    
    if (cmd["scenario_chunk"].is<JsonVariant>()) {
        JsonObject chunk = cmd["scenario_chunk"];
        String id = chunk["id"];
        
        if (id == _scenarioId) {
            String chunkData = chunk["chunk_data"];
            _buffer += chunkData;
            _receivedChunks++;
            
            println("Chunk " + String(_receivedChunks) + "/" + String(_targetChunks), "CMD");
        }
    }
    
    if (cmd["scenario_end"].is<JsonVariant>()) {
        JsonObject end = cmd["scenario_end"];
        String id = end["id"];
        
        if (id == _scenarioId && _receivedChunks == _targetChunks) {
            println(vert("Scenario received complete!"), "CMD");
            
            // Parser le JSON reconstruit
            JsonDocument scenario;
            deserializeJson(scenario, _buffer);
            
            // Appliquer
            _leds->newScenario(scenario.as<JsonObject>());
            
            // Nettoyer
            _buffer = "";
            _scenarioId = "";
        } else {
            println(rouge("Error: chunks mismatch!"), "CMD");
        }
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
