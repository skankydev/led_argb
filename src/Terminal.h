// Terminal.h
#ifndef DEF_TERMINAL
#define DEF_TERMINAL

#include "setting.h"
#include "LedManager.h"

class Terminal {
	private:
		static Terminal* instance;
		Terminal();
		
		String _currentCmd;
		LedManager* _leds;

		void processCommand(String cmd);
		void printLigne(const String& commande, const String& params, const String& aide, const String& couleur = "");
		void cmdHelp();
	

	public:
		static Terminal* getInstance();
		void step();

};

#endif