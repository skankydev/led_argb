#include "setting.h"
#include "MainApp.h"
#include <esp_task_wdt.h>

MainApp mainApp;

void setup() {
	esp_task_wdt_init(60, true); // watchdog 60s, panic + reboot si dépassé
	esp_task_wdt_add(NULL);
	mainApp.init();
}


void loop() {
	esp_task_wdt_reset();
	mainApp.step();
	//mainApp.testEffects();
}
