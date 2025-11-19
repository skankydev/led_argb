#ifndef DEF_Squelette
#define DEF_Squelette

#include "setting.h"


class Squelette{

	private:
		static Squelette* instance;
		Squelette();

	public:
		static Squelette* getInstance();
};

#endif
