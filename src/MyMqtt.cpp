#include "MyMqtt.h"
#include "CmdManager.h"
#include "WiFiTool.h"


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

	warning("MQTT Connecting");
	Serial.println(jaune(String(MQTT_SERVER))+" - "+jaune(String(MQTT_PORT)));
	uint8_t count = 0;
	this->readConf();

	/*uint64_t chipid = ESP.getEfuseMac();
	String clientId = "ESP32-" + String((uint32_t)chipid, HEX);
*/
	while (!_mqttClient.connect(_login.c_str(), "", "") && count < 10) {
		Serial.print(".");
		delay(1000);
		count++;
	}

	if(_mqttClient.connected()){
		success("MQTT Connected!");
		bool subOk = _mqttClient.subscribe(_topicCmd,0);
		Serial.println("Subscribe to: " + bleu(_topicCmd) + " -> " + (subOk ? vertVif("OK") : rouge("FAILED")));

		//Serial.println(bleuVif("Topic Config")+" : "+vert(_topicCmd));
		//LedManager::getInstance()->mqttOk();
	}else{
		error("MQTT not Connected!");
		//LedManager::getInstance()->wifiKo();
	}

	return true;
}

void MyMqtt::send(String message){
	Serial.println(cyan("Mqtt")+jaune(" SEND : ") + _topicMessage + " - " + message);
	_mqttClient.publish(_topicMessage, message);
}

bool MyMqtt::connected(){
	return _mqttClient.connected();
}

bool MyMqtt::disconnect(){
	_mqttClient.unsubscribe(_topicCmd);
	return _mqttClient.disconnect();
}