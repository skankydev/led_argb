#include "Terminal.h"



Terminal* Terminal::instance = nullptr;

Terminal* Terminal::getInstance() {
	if (!instance) {
		instance = new Terminal;
	}
	return instance;
}

Terminal::Terminal() {
	_leds = LedManager::getInstance();
	setupMap();
}

void Terminal::setupMap() {
	// Commandes sans paramètre
	_command["help"] = &Terminal::help;
	_command["print"] = &Terminal::print;
	_command["reboot"] = &Terminal::reboot;
	_command["default"] = &Terminal::setDefault;
	_command["demo"] = &Terminal::demo;
	_command["noel"] = &Terminal::noel;
	_command["noelBis"] = &Terminal::noelBis;
	_command["next"] = &Terminal::next;
	_command["b+"] = &Terminal::upBrightness;
	_command["b-"] = &Terminal::downBrightness;
	_command["auto"] = &Terminal::toggleAutoMode;



	// Commandes avec paramètre
	_commandParam["setColor"] = &Terminal::setColor;
	_commandParam["setBrightness"] = &Terminal::setBrightness;
	_commandParam["setMode"] = &Terminal::setMode;
	_commandParam["setAutoDelay"] = &Terminal::setAutoDelay;

}

void Terminal::step() {
	String cmd = "";
	if (Serial.available() > 0) {
		cmd = Serial.readStringUntil('\n');
		cmd.trim();
		if (cmd.length() > 0) {
			doCommand(cmd);
		}
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
		paramsPart = "{"+jauneVif(params)+"}";
	}
	
	int espacesParams = 20 - params.length() - 4;
	for(int i = 0; i < espacesParams; i++) {
		paramsPart += " ";
	}
	
	Serial.println(cmdPart + paramsPart + ": " + aide);
}

void Terminal::doCommand(String cmd) {
	cmd.trim();

	int spaceIdx = cmd.indexOf(' ');
	String baseCmd = (spaceIdx == -1) ? cmd : cmd.substring(0, spaceIdx);
	String params = (spaceIdx == -1) ? "" : cmd.substring(spaceIdx + 1);

	// Commandes sans paramètre
	auto it = _command.find(baseCmd);
	if (it != _command.end()) {
		(this->*it->second)();
		return;
	}

	// Commandes avec paramètre
	auto itParam = _commandParam.find(baseCmd);
	if (itParam != _commandParam.end()) {
		(this->*itParam->second)(params);
		return;
	}

	// Commande inconnue
	Serial.println(jaune("━━━━━━━━ ") + "Cmd inconnue " + rougeVif(baseCmd) + jaune(" ━━━━━━━━"));
}


void Terminal::help() {
	Serial.println(violet("╔══════════ ")+ vertVif("Terminal Commands")+violet(" ══════════"));
	printLigne("help","","Affiche cette aide","bleu");
	printLigne("print","","Affiche cette aide","bleu");
	printLigne("reboot","","Comme son nom l'indique","rouge");
	printLigne("default","","reset config","vert");
	printLigne("demo","","demo de quelque effet","vert");
	printLigne("noel", "", "Effet Noel twinkle rouge/vert", "vert");
	printLigne("noelBis", "", "Effet Noel fire flicker", "vert");
	printLigne("next", "", "passe aui mode suivant", "vert");
	printLigne("auto", "", "active/desactive le mode auto", "vert");
	printLigne("b+", "", "augment la luminosité", "vert");
	printLigne("b-", "", "diminu la luminosité", "vert");
	printLigne("setBrightness", "1-100", "Définit la luminosité", "vert");
	printLigne("setMode", "1-71", "Définit la luminosité", "vert");
	printLigne("setColor", "RRGGBB", "Définit la couleur (ex: FF0000)", "vert");
	Serial.println(violet("╚══════════════════════════════════════"));
}

//
void Terminal::print() {
	_leds->print();
	_leds->printMode();
}

void Terminal::reboot() {
	Serial.println(jauneVif("Redémarrage du système..."));
	delay(1000);
	ESP.restart();
}

void Terminal::setDefault() {
	_leds->setDefault();
}

void Terminal::demo() {
	_leds->demo();
}

void Terminal::noel() {
	_leds->noel();
}

void Terminal::noelBis() {
	_leds->noelBis();
}

void Terminal::next(){
	_leds->setNextEffect();
}

void Terminal::setColor(String params) {
	if (params.length() == 6) {
		char colorStr[7] = "0x";
		params.toCharArray(colorStr + 2, 7);
		uint32_t color = strtol(colorStr, NULL, 16);
		_leds->setColor(color);
		Serial.println(vert("Couleur définie sur 0x" + params));
	} else {
		Serial.println(rouge("Format invalide. Utilisez : setcolor RRGGBB"));
	}
}

void Terminal::upBrightness(){
	int b = _leds->getBrightness();
	b += 10;
	if(b > 150){
		b = 1;
	}
	_leds->setBrightness(b);

}
void Terminal::downBrightness(){
	int b = _leds->getBrightness();
	b -= 10;
	if(b < 1){
		b = 149;
	}
	_leds->setBrightness(b);
}

void Terminal::setBrightness(String params){
	uint8_t b = params.toInt();
	_leds->setBrightness(b);
}

void Terminal::setMode(String params){
	uint32_t m = params.toInt();
	if(m > 71 || m < 0){
		return error("Mode inconnue");
	}
	_leds->setEffect(m);
}

void Terminal::toggleAutoMode(){
	_leds->toggleAutoMode();
}

void Terminal::setAutoDelay(String params){
	uint32_t d = params.toInt();
	_leds->setAutoDelay(d);
}