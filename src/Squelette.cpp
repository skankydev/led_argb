#include "Squelette.h"

Squelette *Squelette::instance = nullptr;

Squelette *Squelette::getInstance()
{
	if (!instance) {
		instance = new Squelette;
	}
	return instance;
}

Squelette::Squelette() {
}
