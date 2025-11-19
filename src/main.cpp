#include "setting.h"
#include "MainApp.h"


MainApp mainApp;

void setup() {
	mainApp.init();
}


void loop() {
	mainApp.step();
	//mainApp.testEffects();
}
