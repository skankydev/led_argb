#pragma once

#include "setting.h"
#include "Terminal.h"
#include "LedManager.h"
#include "WiFiTool.h"
#include "MyMqtt.h"

class MainApp {

	private:

		LedManager* _leds;
		Terminal* _terminal;
		WiFiTool* _wifi;
		MyMqtt* _mqtt;

	public:
		MainApp();
		void init();
		void step();
};
