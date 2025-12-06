#ifndef DEF_WIFITOOL
#define DEF_WIFITOOL

#include "setting.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <Ticker.h>
#include <vector>
#include <HTTPUpdate.h>

class WiFiTool {
	private:
		static WiFiTool* instance;
		WiFiTool();

		String _name;
		String _pass;

		bool _isLoged = false;

	public:
		static WiFiTool* getInstance();
		
		String getName();
		String getPass();
		String getSSID();
		bool connected();

		

		void readConfig();

		void connect();
		void disconnect();
		void reset();

		String scanNetworks();
		String update(String file);

};


#endif

