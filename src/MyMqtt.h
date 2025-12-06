#ifndef DEF_MYMQTT
#define DEF_MYMQTT

#include "setting.h"
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>

class MyMqtt{

	private:
		static MyMqtt* instance;
		MyMqtt();

		WiFiClient _wifiClient;

		MQTTClient _mqttClient;
		String _login;
		String _topicMesure;
		String _topicConfig;
		bool _lastConnected;
		uint64_t _timoutReco;
		uint64_t _timoutOldMessage;
		uint64_t _lastReconnect = 0;

	public:
		static MyMqtt* getInstance();

		void init();
		void readConf();
		bool connect();
		bool connected();
		bool disconnect();

		void step();
		void send(String message);
};

#endif
