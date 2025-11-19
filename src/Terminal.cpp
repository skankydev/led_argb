#include "Terminal.h"



Terminal* Terminal::instance = nullptr;

Terminal* Terminal::getInstance() {
	if (!instance) {
		instance = new Terminal;
	}
	return instance;
}

Terminal::Terminal() {
	_currentCmd = "";
	_leds = LedManager::getInstance();
}

void Terminal::step() {
	while (Serial.available() > 0) {
		_currentCmd = Serial.readString();
	}
	_currentCmd.trim();
	
	if(_currentCmd.length() > 0) {
		this->processCommand(_currentCmd);
		_currentCmd = "";
	}
}

void Terminal::printLigne(const String& commande, const String& params, const String& aide, const String& couleur) {
	String cmdColored = commande;
	if (couleur == "bleu") {
		cmdColored = bleu(commande);
	} else if (couleur == "rouge") {
		cmdColored = rouge(commande);
	} else if (couleur == "vert") {
		cmdColored = vert(commande);
	} else if (couleur == "jaune") {
		cmdColored = jauneVif(commande);
	}
	
	String cmdPart = violet("║ ")+ cmdColored;
	
	int espacesCmd = 15 - commande.length();
	for(int i = 0; i < espacesCmd; i++) {
		cmdPart += " ";
	}
	
	String paramsPart = "";
	if (params.length() > 0) {
		paramsPart = jauneVif(params);
	}
	
	int espacesParams = 20 - params.length();
	for(int i = 0; i < espacesParams; i++) {
		paramsPart += " ";
	}
	
	Serial.println(cmdPart + paramsPart + ": " + aide);
}

void Terminal::processCommand(String cmd) {

	if(cmd == "help") {
		this->cmdHelp();
	}else if(cmd.startsWith("reboot")) {
		Serial.println(jauneVif("Redémarrage du système..."));
		delay(1000);
		ESP.restart();
	} else if(cmd == "demo") {
		_leds->demo();
	} else if(cmd == "default") {
		_leds->setDefault();
	} else if(cmd == "noel") {
		_leds->noel();
	} else if(cmd == "noelBis") {
		_leds->noelBis();
	} else if(cmd == "print") {
		_leds->print();
	} else {
		Serial.println(jaune("━━━━━━━━ ")+" Cmd inconue "+rougeVif(cmd)+jaune(" ━━━━━━━━"));
	}
	
	
	// Ajouter vos nouvelles commandes ici
}

void Terminal::cmdHelp() {
	Serial.println(violet("╔══════════ ")+ vertVif("Terminal Commands")+violet(" ══════════"));
	printLigne("help","","Affiche cette aide","bleu");
	printLigne("print","","Affiche cette aide","bleu");
	printLigne("reboot","","Comme son nom l'indique","rouge");
	printLigne("default","","reset config","vert");
	printLigne("demo","","demo de quelque effet","vert");
	printLigne("noel", "", "Effet Noel twinkle rouge/vert", "vert");
	printLigne("noelBis", "", "Effet Noel fire flicker", "vert");
	Serial.println(violet("╚══════════════════════════════════════"));
}


