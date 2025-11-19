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

	error("Init ready");
}

/**
 * Loop
 * @return {void} rien
 */
void MainApp::step(){
	_leds->step();
	_terminal->step();
}
