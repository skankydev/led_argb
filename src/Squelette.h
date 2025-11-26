#pragma once

#include "setting.h"


class Squelette{

	private:
		static Squelette* instance;
		Squelette();

	public:
		static Squelette* getInstance();
};
