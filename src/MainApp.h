#pragma once

#include "setting.h"
#include <LittleFS.h>
#include "Terminal.h"
#include "LedManager.h"
#include "WiFiTool.h"
#include "MyMqtt.h"
#include "CmdManager.h"

class MainApp {

	private:

		LedManager* _leds;
		Terminal* _terminal;
		WiFiTool* _wifi;
		MyMqtt* _mqtt;
		CmdManager* _cmd;

	public:
		MainApp();
		void init();
		void step();
};
