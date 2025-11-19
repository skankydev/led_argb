#ifndef DEF_MAINAPP
#define DEF_MAINAPP

#include "setting.h"
#include "Terminal.h"
#include "LedManager.h"


class MainApp {

	private:

		LedManager* _leds;
		Terminal* _terminal;

	public:
		MainApp();
		void init();
		void step();
};


#endif