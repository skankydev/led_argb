#include "MainApp.h"

/**
 * constructeur
 */
MainApp::MainApp() {

}

/**
 * initialisation
 * @return {void} rien
 */
void MainApp::init(){
	Serial.begin(115200);
	success("LED ARGB - WS2812FX");

	int reason = esp_reset_reason();
	String message = getResetReason(reason);
	info(message);

	_leds = LedManager::getInstance();
	_terminal = Terminal::getInstance();

	_wifi = WiFiTool::getInstance();
	//_wifi->connect();

	_mqtt = MyMqtt::getInstance();
	_mqtt->init();
	_mqtt->connect();

	error("Init ready");
	//_leds->custome();
}

/**
 * Loop
 * @return {void} rien
 */
void MainApp::step(){
	_leds->step();
	_terminal->step();
	_mqtt->step();
}
