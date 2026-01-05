#include "MyMqtt.h"
#include "CmdManager.h"
#include "WiFiTool.h"


void mqttReceived(String &topic, String &message) {
	println(vert("RECIVE : ") + topic + " - " + message,"MQTT");
	CmdManager::getInstance()->newCmd(message);
}



MyMqtt *MyMqtt::instance = nullptr;

MyMqtt *MyMqtt::getInstance() {
	if (!instance) {
		instance = new MyMqtt;
	}
	return instance;
}

MyMqtt::MyMqtt() : _wifiClient(), _mqttClient(4096)  {}

void MyMqtt::init(){
	_mqttClient.setKeepAlive(60);
	_mqttClient.setCleanSession(true);
	_mqttClient.begin(MQTT_SERVER,MQTT_PORT,_wifiClient);
	_mqttClient.onMessage(mqttReceived);

	_timoutReco = millis();
	_timoutReco += RECO_TIMER;

	this->readConf();
	success("Mqtt init");
}

void MyMqtt::readConf(){
	_login = "led_test";
	_topicMessage = "skankyhome/info/"+_login;
	_topicCmd = "skankyhome/cmd/"+_login;
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

	warning("MQTT Connecting","MQTT");
	println(jaune(String(MQTT_SERVER))+" - "+jaune(String(MQTT_PORT)),"MQTT");
	uint8_t count = 0;
	this->readConf();

	while (!_mqttClient.connect(_login.c_str(), "", "") && count < 10) {
		Serial.print(".");
		delay(1000);
		count++;
	}
	Serial.print("\n");

	if(_mqttClient.connected()){
		success("MQTT Connected!","MQTT");
		bool subOk = _mqttClient.subscribe(_topicCmd,0);
		println("Subscribe to: " + bleu(_topicCmd) + " -> " + (subOk ? vertVif("OK") : rouge("FAILED")),"MQTT");
	}else{
		error("MQTT not Connected!","MQTT");
	}

	return true;
}

void MyMqtt::send(String message){
	println(jaune("SEND : ") + _topicMessage + " - " + message,"MQTT");
	_mqttClient.publish(_topicMessage, message);
}

bool MyMqtt::connected(){
	return _mqttClient.connected();
}

bool MyMqtt::disconnect(){
	_mqttClient.unsubscribe(_topicCmd);
	return _mqttClient.disconnect();
}