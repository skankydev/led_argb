#include "MyMqtt.h"
#include "DB/Table/ConfigTable.h"
#include "CmdManager.h"
#include "LedManager.h"
#include "WiFiTool.h"
#include "MyRtc.h"
#include "DB/Table/MessageTable.h"

void mqttReceived(String &topic, String &message) {
	Serial.println(cyan("Mqtt")+vert(" RECIVE : ") + topic + " - " + message);
	CmdManager::getInstance()->newCmd(message);
}



MyMqtt *MyMqtt::instance = nullptr;

MyMqtt *MyMqtt::getInstance() {
	if (!instance) {
		instance = new MyMqtt;
	}
	return instance;
}

MyMqtt::MyMqtt() : _wifiClient(), _mqttClient(1024)  {}

void MyMqtt::init(){
	_mqttClient.setKeepAlive(60);
	_mqttClient.setCleanSession(true);
	_mqttClient.begin(MQTT_SERVER,MQTT_PORT,_wifiClient);
	_mqttClient.onMessage(mqttReceived);

	_timoutReco = millis();
	_timoutReco += RECO_TIMER;

	this->readConf();
}

void MyMqtt::readConf(){
	_login = "led_master";
	_topicMesure = "skankyhome/info/"+_login;
	_topicConfig = "skankyhome/cmd/"+_login;
}


void MyMqtt::step(){
	if (!WiFiTool::getInstance()->connected()) {
		if (millis() > _timoutReco) {
			_timoutReco = millis();
			_timoutReco += RECO_TIMER;
			WiFiTool::getInstance()->connect();
		}
		return;
	}
	_mqttClient.loop();

	if (!_mqttClient.connected()) {
		if (millis() > _timoutReco) {
			_timoutReco = millis();
			_timoutReco += RECO_TIMER;
			warning("Tentative de reconnexion MQTT...");
			this->connect();
		}
	}
}

bool MyMqtt::connect(){

	warning("MQTT Connecting");
	uint8_t count;
	this->readConf();
	while (!_mqttClient.connect(String(ESP.getEfuseMac()).c_str(), "", "") && count < 10) {
		Serial.print(".");
		delay(1000);
		count++;
	}

	if(_mqttClient.connected()){
		success("MQTT Connected!");
		_mqttClient.subscribe(_topicConfig);
		LedManager::getInstance()->mqttOk();
	}else{
		error("MQTT not Connected!");
		LedManager::getInstance()->wifiKo();
	}

	return true;
}

void MyMqtt::send(String message){
	Serial.println(cyan("Mqtt")+jaune(" SEND : ") + _topicMesure + " - " + message);
	_mqttClient.publish(_topicMesure, message);
}

bool MyMqtt::connected(){
	return _mqttClient.connected();
}

bool MyMqtt::disconnect(){
	_mqttClient.unsubscribe(_topicConfig);
	return _mqttClient.disconnect();
}