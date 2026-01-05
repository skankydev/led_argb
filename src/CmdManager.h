#pragma once

#include "setting.h"
#include "LedManager.h"

class CmdManager{

	private:
		static CmdManager* instance;
		CmdManager();

		LedManager* _leds;
		String _scenarioId = "";
		String _buffer = "";
		int _targetChunks = 0;
		int _receivedChunks = 0;

	public:
		static CmdManager* getInstance();

		void newCmd(String message); 

		void sayHello();
};
