#include "WiFiTool.h"
#include "Config.h"
#include "CmdManager.h"
#include "LedManager.h"
#include "DB/Table/ConfigTable.h"

void WiFiEvent(WiFiEvent_t event) {
	//Serial.println(vert("[Lan-event]")+ " event : "+vertVif(String(event)));
	switch (event) {
		case ARDUINO_EVENT_WIFI_READY:
			success("WiFi interface ready");
			break;

		case ARDUINO_EVENT_WIFI_SCAN_DONE:
			info("Completed scan for access points");
			break;

		case ARDUINO_EVENT_WIFI_STA_START:
			//warning("WiFi client started");
			break;

		case ARDUINO_EVENT_WIFI_STA_STOP:
			warning("WiFi clients stopped");
			break;

		case ARDUINO_EVENT_WIFI_STA_CONNECTED:
			info("Connected to access point");
			break;

		case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
			error("Disconnected from WiFi access point");
			LedManager::getInstance()->wifiKo();
			break;

		case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE: 
			//warning("Authentication mode of access point has changed"); 
			break;

		case ARDUINO_EVENT_WIFI_STA_GOT_IP:
			Serial.println("WiFi IP : "+vert(getIpString(WiFi.localIP())));
			Serial.println("WiFi Mac : "+WiFi.macAddress() );
			break;

		case ARDUINO_EVENT_WIFI_STA_LOST_IP:        
			error("Lost IP address and IP address is reset to 0"); 
			break;
		case ARDUINO_EVENT_WPS_ER_SUCCESS:
			success("WiFi Protected Setup (WPS): succeeded in enrollee mode"); 
			break;
		case ARDUINO_EVENT_WPS_ER_FAILED:
			error("WiFi Protected Setup (WPS): failed in enrollee mode"); 
			break;
		case ARDUINO_EVENT_WPS_ER_TIMEOUT:
			warning("WiFi Protected Setup (WPS): timeout in enrollee mode"); 
			break;
		case ARDUINO_EVENT_WPS_ER_PIN:
			warning("WiFi Protected Setup (WPS): pin code in enrollee mode"); 
			break;
		case ARDUINO_EVENT_WIFI_AP_START:
			info("WiFi access point started"); 
			break;
		case ARDUINO_EVENT_WIFI_AP_STOP:
			info("WiFi access point stopped"); 
			break;
		case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
			success("Client connected"); 
			break;
		case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
			LedManager::getInstance()->wifiKo();
			error("Client disconnected"); 
			break;
		case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
			info("Assigned IP address to client"); 
			break;
		case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
			Serial.println("Received probe request"); 
			break;

		case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
			Serial.println("AP IPv6 is preferred"); 
			break;
		case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
			Serial.println("STA IPv6 is preferred"); 
			break;
		case ARDUINO_EVENT_ETH_GOT_IP6:
			Serial.println("Ethernet IPv6 is preferred"); 
			break;
		default:
			break;
	}
}

void update_started() {
	Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
	Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
	Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
	Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}

WiFiTool* WiFiTool::instance = nullptr;
/**
 * retourn le pointeur du singleton
 */
WiFiTool* WiFiTool::getInstance(){
	if(!instance){
		instance = new WiFiTool;
	}
	return instance;
}


WiFiTool::WiFiTool() {
	WiFi.setHostname("led_master");
	this->readConfig();
	
	WiFi.onEvent(WiFiEvent);

	this->connect();
}

void WiFiTool::readConfig() {
	_name = WIFI_SSID;
	_name.trim();
	_pass = WIFI_PASSWORD;
	_pass.trim();
}

String WiFiTool::getName() {
	return _name;
}

String WiFiTool::getPass() {
	return _pass;
}

String WiFiTool::getSSID() {
	return String(WiFi.SSID());
}

bool WiFiTool::connected() {
	return WiFi.status() == WL_CONNECTED;
}

/**
 * connextion au reseau wifi
 * @return void
 */
void WiFiTool::connect() {

	Serial.println(vert("Connexion au réseau WiFi ")+bleuVif(_name)+" - "+rougeVif(_pass));
	if(_name.length() == 0){
		this->readConfig();
		if(_name.length() == 0){
			error("Wifi non configuré");
			return;
		}
	}
	WiFi.begin(_name,_pass);
	int count = 0;
	Serial.println(" ");
	while (WiFi.status() != WL_CONNECTED && count < 10) {
		delay(1000);
		Serial.print(bleu("."));
		count++;
	}
	Serial.println(" ");
	if(WiFi.status() != WL_CONNECTED){
		error("Wifi Marche pas");
		if(!_isLoged){
			_isLoged = true;
			logError("[wifi] Not Connected");
		}
	}else{
		_isLoged = false;
		logInfo("[wifi] Connected");
	}

}

/**
 * deconection du reseau wifi
 * @return void
 */
void WiFiTool::disconnect(){
	WiFi.disconnect();
}


/**
 * Effacer la configuration du reseau wifi
 * @return void
 */
void WiFiTool::reset() {}


String WiFiTool::scanNetworks(){
	Serial.println("Scan des réseaux WiFi disponibles...");
	int numReseaux = WiFi.scanNetworks();
	String result = "";

	if (numReseaux == 0) {
		Serial.println("Aucun réseau trouvé.");
	} else {
		Serial.print(numReseaux);
		Serial.println(" réseaux trouvés :");

		for (int i = 0; i < numReseaux; ++i) {
			Serial.println(String(i)+": " + String(WiFi.SSID(i)) + " (" + String(WiFi.RSSI(i)) + ")");
			result += WiFi.SSID(i)+",";
		}
		result.remove(result.length() - 1);
	}
	return result;
}


String WiFiTool::update(String file) {

	String url = "";
	#ifdef HTTPS
		WiFiClientSecure client;
		client.setInsecure();
		url = "https://"+String(SERVER);
	#else
		WiFiClient client;
		url = "http://"+String(SERVER);
	#endif

	httpUpdate.onStart(update_started);
    httpUpdate.onEnd(update_finished);
    httpUpdate.onProgress(update_progress);
    httpUpdate.onError(update_error);

	String detination = url+file ;
	info("Updat : "+detination);

	t_httpUpdate_return ret = httpUpdate.update(client, detination);

	switch (ret) {
		case HTTP_UPDATE_FAILED:
			error("HTTP_UPDATE_FAILED Error");
			break;

		case HTTP_UPDATE_NO_UPDATES:
			info("HTTP_UPDATE_NO_UPDATES");
			break;

		case HTTP_UPDATE_OK:
			success("HTTP_UPDATE_OK");
			break;
	}
	return String("200");

}
