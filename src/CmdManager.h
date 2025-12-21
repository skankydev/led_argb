#pragma once

#include "setting.h"
#include "LedManager.h"

class CmdManager{

	private:
		static CmdManager* instance;
		CmdManager();

		LedManager* _leds;

	public:
		static CmdManager* getInstance();

		void newCmd(String message); 

		void sayHello();
};
