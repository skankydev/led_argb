#ifndef DEF_LEDMANAGER
#define DEF_LEDMANAGER

#include "setting.h"
#include <WS2812FX.h>

class LedManager{

	private:
		static LedManager* instance;
		LedManager();


		WS2812FX _ligne1;

	public:
		static LedManager* getInstance();

		void step();

		void setDefault();
		void setEffect(uint8_t mode, String target = "all");
		void setColor(uint32_t color, String target = "all");
		void setSpeed(uint16_t speed, String target = "all");
		void setBrightness(uint8_t brightness, String target = "all");
		void demo();

		void noel();
		void noelBis();

		void run();
		void blink();
		void rainbow();
		void scanner();
		void theaterChaseRainbow();
		void print();
};

#endif
