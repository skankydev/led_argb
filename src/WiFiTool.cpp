#include "WiFiTool.h"
#include "CmdManager.h"

void WiFiEvent(WiFiEvent_t event) {
	//Serial.println(vert("[Lan-event]")+ " event : "+vertVif(String(event)));
	switch (event) {
		case ARDUINO_EVENT_WIFI_READY:
			//success("WiFi interface ready");
			break;

		case ARDUINO_EVENT_WIFI_SCAN_DONE:
			info("Completed scan for access points");
			break;

		case ARDUINO_EVENT_WIFI_STA_START:
			warning("WiFi client started","WIFI");
			break;

		case ARDUINO_EVENT_WIFI_STA_STOP:
			warning("WiFi clients stopped","WIFI");
			break;

		case ARDUINO_EVENT_WIFI_STA_CONNECTED:
			info("Connected to access point","WIFI");
			break;

		case ARDUINO_EVENT_WIFI_STA_GOT_IP:
			println("WiFi IP : "+vert(WiFi.localIP().toString()),"WIFI");
			println("WiFi Mac : "+WiFi.macAddress() ,"WIFI");
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
	Serial.println("CALLBACK: HTTP update process started");
}

void update_finished() {
	Serial.println("CALLBACK: HTTP update process finished");
}

void update_progress(int cur, int total) {
	Serial.printf("CALLBACK: HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
	Serial.printf("CALLBACK: HTTP update fatal error code %d\n", err);
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

	println(vert("Connexion au réseau WiFi ")+bleuVif(_name)+" - "+rougeVif(_pass),"WIFI");
	if(_name.length() == 0){
		this->readConfig();
		if(_name.length() == 0){
			error("Wifi non configuré","WIFI");
			return;
		}
	}
	WiFi.begin(_name,_pass);
	int count = 0;
	Serial.println(" ");
	while (WiFi.status() != WL_CONNECTED && count < 10) {
		delay(1000);
		//Serial.println(bleu("."));
		count++;
	}
	Serial.println(" ");
	if(WiFi.status() != WL_CONNECTED){
		error("Wifi Marche pas","WIFI");
		if(!_isLoged){
			_isLoged = true;
		}
	}else{
		_isLoged = false;
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
	warning("Scan des réseaux WiFi disponibles...","WIFI");
	int numReseaux = WiFi.scanNetworks();
	String result = "";

	if (numReseaux == 0) {
		error("Aucun réseau trouvé.","WIFI");
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
	WiFiClient client;
	url = "http://"+String(WEB_SERVER);

	httpUpdate.onStart(update_started);
	httpUpdate.onEnd(update_finished);
	httpUpdate.onProgress(update_progress);
	httpUpdate.onError(update_error);

	String detination = url+file ;
	info("Updat : "+detination);

	t_httpUpdate_return ret = httpUpdate.update(client, detination);

	switch (ret) {
		case HTTP_UPDATE_FAILED:
			error("HTTP_UPDATE_FAILED Error","WIFI");
			break;

		case HTTP_UPDATE_NO_UPDATES:
			info("HTTP_UPDATE_NO_UPDATES","WIFI");
			break;

		case HTTP_UPDATE_OK:
			success("HTTP_UPDATE_OK","WIFI");
			break;
	}
	return String("200");

}
