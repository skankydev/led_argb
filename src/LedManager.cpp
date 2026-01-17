#include "LedManager.h"
#include "DataSaver.h"

LedManager* LedManager::instance = nullptr;

LedManager* LedManager::getInstance() {
	if (!instance) {
		instance = new LedManager;
	}
	return instance;
}

LedManager::LedManager() : _scenario() {
	// Créer les 3 lignes
	_lines["0"] = new LedConfig(PIN_LEDS_0, NUM_LEDS);
	_lines["1"] = new LedConfig(PIN_LEDS_1, NUM_LEDS);
	_lines["2"] = new LedConfig(PIN_LEDS_2, NUM_LEDS);
	
	// Init toutes les lignes
	for(auto& [name, config] : _lines) {
		config->init();
	}
	
	_lastChange = 0;
	_autoDelay = 5000;
	
	_scenarioMode = this->initScenario();
}

bool LedManager::initScenario() {
	String content = DataSaver::read("scenario.json");
	DeserializationError err = deserializeJson(_scenario, content);
	if (err) {
		warning("Scenario deserialize failed ");
		error(err.c_str());
		return false;
	}
	success("Scenario Ok");
	return true;
}

void LedManager::newScenario(JsonObject scenario) {
	String content;
	serializeJson(scenario, content);
	DataSaver::save("scenario.json", content);
	_scenarioMode = this->initScenario();
}

void LedManager::step() {
	// Service de toutes les lignes
	for(auto& [name, config] : _lines) {
		config->service();
	}

	if(_scenarioMode) {
		unsigned long now = millis();
		
		// Gérer chaque ligne indépendamment
		for(auto& [name, config] : _lines) {
			String lineName = "line_" + name;
			
			if(config->needsUpdate(now)) {
				// Vérifier si la step existe
				if(!_scenario[lineName]["steps"][config->getKey()].is<JsonVariant>()) {
					config->resetKey();
				}
				
				// Charger et appliquer la step
				JsonObject step = _scenario[lineName]["steps"][config->getKey()];
				config->applyStep(step);
				config->updateLastChange(now);
			}
		}
	}
}

/*void LedManager::print() {
	// Afficher les infos de toutes les lignes
	for(auto& [name, config] : _lines) {
		Serial.println(violet("╭───────────────────────"));
		Serial.println(violet("│ Ligne ") + jaune(name));
		Serial.print(violet("│ Mode")+"       : ");
		Serial.println(jaune(config->getModeName(config->getMode())));

		// Affiche la couleur en hexa
		uint32_t color = config->getColor();
		Serial.print(violet("│ Couleur")+"    : ");
		uint8_t r = (color >> 16) & 0xFF;
		uint8_t g = (color >> 8)  & 0xFF;
		uint8_t b = color & 0xFF;
		char colorStr[7];
		snprintf(colorStr, sizeof(colorStr), "%02X%02X%02X", r, g, b);
		Serial.println(jaune("0x"+String(colorStr)));

		// Affiche la vitesse
		Serial.print(violet("│ Vitesse")+"    : ");
		Serial.println(jaune(String(config->getSpeed())));

		// Affiche la luminosité
		Serial.print(violet("│ Luminosité")+" : ");
		Serial.println(jaune(String(config->getBrightness())));
		Serial.println(violet("╰───────────────────────"));
		Serial.println();
	}
}
*/
void LedManager::printMode() {
	Serial.println(bleu("╭────────── ")+ "Mode" +bleu(" ─────────────"));
	
	// On prend juste la première ligne pour afficher les modes disponibles
	LedConfig* config = _lines["0"];
	for (int i = 0; i < 80; ++i) {
		Serial.println(bleu("│ ")+rouge(String(i))+ " => " + config->getModeName(i));
	}
	Serial.println(bleu("╰──────────────────────────"));
}

void LedManager::setDefault() {
	for(auto& [name, config] : _lines) {
		config->setDefault();
	}
}

void LedManager::setNextEffect(String target) {
	if(target == "all") {
		for(auto& [name, config] : _lines) {
			uint32_t mode = config->getMode();
			mode += 1;
			if(mode > 71) {
				mode = 0;
			}
			config->setEffect(mode);
			println(bleu("Mode Ligne "+name) + " : " + rouge(String(mode))+ " -> " + jaune(config->getModeName(mode)),"LED");
		}
	} else if(_lines.find(target) != _lines.end()) {
		uint32_t mode = _lines[target]->getMode();
		mode += 1;
		if(mode > 71) {
			mode = 0;
		}
		_lines[target]->setEffect(mode);
		println(bleu("Mode Ligne "+target) + " : " + rouge(String(mode))+ " -> " + jaune(_lines[target]->getModeName(mode)),"LED");
	}
}

void LedManager::setEffect(uint8_t mode, String target) {
	if(target == "all") {
		for(auto& [name, config] : _lines) {
			config->setEffect(mode);
		}
		println(bleu("Mode") + " : " + rouge(String(mode))+ " -> " + jaune(_lines["0"]->getModeName(mode)),"LED");
	} else if(_lines.find(target) != _lines.end()) {
		_lines[target]->setEffect(mode);
		println(bleu("Mode Ligne "+target) + " : " + rouge(String(mode))+ " -> " + jaune(_lines[target]->getModeName(mode)),"LED");
	}
}

void LedManager::setColor(uint32_t color, String target) {
	if(target == "all") {
		for(auto& [name, config] : _lines) {
			config->setColor(color);
		}
	} else if(_lines.find(target) != _lines.end()) {
		_lines[target]->setColor(color);
	}
}

void LedManager::setSpeed(uint16_t speed, String target) {
	if(target == "all") {
		for(auto& [name, config] : _lines) {
			config->setSpeed(speed);
		}
	} else if(_lines.find(target) != _lines.end()) {
		_lines[target]->setSpeed(speed);
	}
}

void LedManager::setBrightness(uint8_t brightness, String target) {
	println(jaune("Brightness")+" : "+String(brightness),"LED");
	if(target == "all") {
		for(auto& [name, config] : _lines) {
			config->setBrightness(brightness);
		}
	} else if(_lines.find(target) != _lines.end()) {
		_lines[target]->setBrightness(brightness);
	}
}

uint8_t LedManager::getBrightness(String target) {
	if(target == "all") {
		// Retourne la brightness de la première ligne
		return _lines["0"]->getBrightness();
	} else if(_lines.find(target) != _lines.end()) {
		return _lines[target]->getBrightness();
	}
	return 0;
}

void LedManager::setSegments(JsonArray segments, String target) {
	if(target == "all") {
		for(auto& [name, config] : _lines) {
			config->setSegments(segments);
		}
	} else if(_lines.find(target) != _lines.end()) {
		_lines[target]->setSegments(segments);
	}
}