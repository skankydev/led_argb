#pragma once

#include <map>
#include "setting.h"
#include "LedManager.h"


class Terminal {
public:
	static Terminal* getInstance();
	void step();

private:
	Terminal();
	static Terminal* instance;
	LedManager* _leds;

	void printLigne(const String& commande, const String& params, const String& aide, const String& couleur = "bleu");

	// Types pour les handlers de commande
	typedef void (Terminal::*CommandHandler)();
	typedef void (Terminal::*CommandHandlerWithParam)(String);
	std::map<String, CommandHandler> _command;
	std::map<String, CommandHandlerWithParam> _commandParam;

	// Méthodes de gestion des commandes
	void setupMap();
	void doCommand(String cmd);

	void help();
	void print();
	void reboot();
	void setDefault();
	void demo();
	void noel();
	void noelBis();
	void next();
	void upBrightness();
	void downBrightness();
	void format();
	void files();
	void system();

	

	void setColor(String params);
	void setBrightness(String params);
	void setMode(String params);

};
