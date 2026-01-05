#include "Terminal.h"
#include "DataSaver.h"


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
	_command["format"] = &Terminal::format;
	_command["files"] = &Terminal::files;
	_command["system"] = &Terminal::system;





	// Commandes avec paramètre
	_commandParam["setColor"] = &Terminal::setColor;
	_commandParam["setBrightness"] = &Terminal::setBrightness;
	_commandParam["setMode"] = &Terminal::setMode;

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
	printLigne("print","","Affiche les information de l esp","bleu");
	printLigne("files","","Affiche tout les fichier","bleu");
	printLigne("system","","Affiche les information System","bleu");
	printLigne("reboot","","Comme son nom l'indique","rouge");
	printLigne("format","","supprime tout les fichier","rouge");
	printLigne("default","","reset config","vert");
	printLigne("demo","","demo de quelque effet","vert");
	printLigne("noel", "", "Effet Noel twinkle rouge/vert", "vert");
	printLigne("noelBis", "", "Effet Noel fire flicker", "vert");
	printLigne("next", "", "passe aui mode suivant", "vert");
	printLigne("b+", "", "augment la luminosité", "vert");
	printLigne("b-", "", "diminu la luminosité", "vert");
	printLigne("setBrightness", "1-100", "Définit la luminosité", "vert");
	printLigne("setMode", "1-71", "Définit la luminosité", "vert");
	printLigne("setColor", "#RRGGBB", "Définit la couleur (ex: FF0000)", "vert");
	Serial.println(violet("╚══════════════════════════════════════"));
}

//
void Terminal::print() {
	_leds->print();
	//_leds->printMode();
	Serial.println(violet("═══════════════")+ vertVif("Scenario")+violet("════════════"));
	String content = DataSaver::read("scenario.json");
	Serial.println(content);

	JsonDocument scenario;
	DeserializationError err = deserializeJson(scenario, content);
	if (err) {
		warning("Scenario deserialize failed ");
		error(err.c_str());
		return;
	}
	serializeJsonPretty(scenario,Serial);

}


void Terminal::reboot() {
	println(jauneVif("Redémarrage du système..."),"CLI");
	delay(1000);
	ESP.restart();
}

void Terminal::files() {
	DataSaver::doListAll();
}

void Terminal::format() {
	DataSaver::doFormat();
}

void Terminal::system() {
	printSystemInfo();
	printAllPartitions();
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
	if (params.length() >= 6) {
		if(params.startsWith("#")){
			params = params.substring(1);
		}
		uint32_t color = strtol(params.c_str(), NULL, 16);
		_leds->setColor(color);
		println(vert("Couleur définie sur")+" : "+bleu(" #" + params),"CLI");
	} else {
		println(rouge("Format invalide. Utilisez : setcolor RRGGBB"),"CLI");
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
