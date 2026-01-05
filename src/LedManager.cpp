#include "LedManager.h"
#include "DataSaver.h"

LedManager *LedManager::instance = nullptr;

LedManager *LedManager::getInstance()
{
	if (!instance) {
		instance = new LedManager;
	}
	return instance;
}

LedManager::LedManager() : _ligne0(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800) , _scenario() {


	_ligne0.init();	
	this->setDefault();
	_ligne0.start();

	_scenarioMode = this->initScenario();

}

bool LedManager::initScenario(){
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

void LedManager::newScenario(JsonObject scenario){
	String content;
	serializeJson(scenario,content);
	DataSaver::save("scenario.json",content);
	_scenarioMode = this->initScenario();
}

void LedManager::step(){
	_ligne0.service();

	if(_scenarioMode){
		unsigned long now = millis();
		if(now - _lastChangeLine0 > _timerLigne0) {
			if(!_scenario["line_0"]["steps"][_keyLine0].is<JsonVariant>()){
				_keyLine0 = 0;
			}
			_timerLigne0 = _scenario["line_0"]["steps"][_keyLine0]["duration"].as<uint64_t>() * 1000;
			this->setSegments(_scenario["line_0"]["steps"][_keyLine0]["segments"].as<JsonArray>());
			_keyLine0++;
			_lastChangeLine0 = now;
		}
	}
}


/**
 * Change l'effet
 */
void LedManager::setNextEffect(String target) {
	uint32_t mode = _ligne0.getMode();
	mode += 1;
	if(mode > 71){
		mode = 0;
	}
	_ligne0.setMode(mode);
	println(bleu("Mode") + " : " + rouge(String(mode))+ " -> " + jaune(String(_ligne0.getModeName(mode))),"LED");
}

/**
 * Change l'effet
 */
void LedManager::setEffect(uint8_t mode, String target) {
	_ligne0.setMode(mode);
	println(bleu("Mode") + " : " + rouge(String(mode))+ " -> " + jaune(String(_ligne0.getModeName(mode))),"LED");
}


/**
 * Change la couleur
 */
void LedManager::setColor(uint32_t color, String target) {
	_ligne0.setColor(color);
}

/**
 * Change la vitesse
 */
void LedManager::setSpeed(uint16_t speed, String target) {
	_ligne0.setSpeed(speed);
}

/**
 * Change la luminosité
 */
void LedManager::setBrightness(uint8_t brightness, String target) {
	println(jaune("Brightness")+" : "+String(brightness),"LED");
	_ligne0.setBrightness(brightness);
}

void LedManager::setSegments(JsonArray segments, String target){
	int id = 0;

	_ligne0.resetSegments();
	_ligne0.resetSegmentRuntimes();

	for(JsonVariant segment : segments) {

		uint16_t first = segment["first"].as<uint16_t>();
		uint16_t last = segment["last"].as<uint16_t>();
		uint8_t effect = segment["effect"].as<uint8_t>();
		uint16_t speed = segment["speed"].as<uint16_t>();
		bool reverse = segment["reverse"].as<bool>();
		
		JsonArray colors = segment["colors"].as<JsonArray>();
		uint32_t color[4];
		color[0] = convertColor(colors[0]) | 0;
		color[1] = convertColor(colors[1]) | 0;
		color[2] = convertColor(colors[2]) | 0;

		_ligne0.setSegment(id, first, last, effect, color, speed, reverse);
		id++;
	}
}

uint8_t LedManager::getBrightness(String target) {
	return _ligne0.getBrightness();
}


void LedManager::run(){
	unsigned long start = millis();
	while(millis() - start < 10000) {
		_ligne0.service();
	}
	this->setDefault();
}

void LedManager::setDefault(){

	_lastChange = 0;
	_autoDelay = 5000; 

	// Effacer tous les segments
	_ligne0.resetSegments();
	_ligne0.resetSegmentRuntimes();
	
	// Recréer un nouveau segment sur tout le bandeau	
	_ligne0.setSegment(0, 0, NUM_LEDS-1, FX_MODE_RAINBOW_CYCLE, 0x0000FF, 1500, false);	
	_ligne0.setBrightness(50);
}

/**
 * Effet Noël - Twinkle Random rouge/vert
 */
void LedManager::noel() {
	_ligne0.setMode(FX_MODE_TWINKLE_RANDOM);
	_ligne0.setColor(RED);
	_ligne0.setSpeed(800);
	success("Mode Noel - Twinkle");
	// Ajoute du vert en couleur secondaire
	uint32_t colors[] = {RED, GREEN, RED};
	_ligne0.setColors(0, colors);
	
	this->run();
}

/**
 * Effet Noël alternatif - Fire Flicker rouge/orange
 */
void LedManager::noelBis() {
	_ligne0.setMode(FX_MODE_FIRE_FLICKER);
	_ligne0.setColor(RED);
	_ligne0.setSpeed(500);
	success("Mode Noel Bis - Fire Flicker");
	this->run();
}

void LedManager::blink() {
	_ligne0.setColor(0x00FF00);
	_ligne0.setMode(FX_MODE_BLINK);
	_ligne0.setSpeed(500);
	info("Blink");
	this->run();
}


void LedManager::rainbow(){
	_ligne0.setMode(FX_MODE_RAINBOW_CYCLE);
	_ligne0.setSpeed(1000);
	info("Rainbow Cycle");
	this->run();
}

void LedManager::scanner(){
	_ligne0.setColor(0xFF0000);
	_ligne0.setMode(FX_MODE_LARSON_SCANNER);
	_ligne0.setSpeed(800);
	info("Larson Scanner");
	this->run();
}


void LedManager::theaterChaseRainbow(){
	_ligne0.setMode(FX_MODE_THEATER_CHASE_RAINBOW);
	_ligne0.setSpeed(1000);
	info("Theater Chase Rainbow");
	this->run();
}

/**
 * Test des effets de base
 */
void LedManager::demo() {
	
	uint32_t tempo = 1000;	

	success("Demo Start");
	this->blink();
	delay(tempo);
	this->rainbow();
	delay(tempo);
	this->scanner();
	delay(tempo);
	this->theaterChaseRainbow();
	success("Demo stop");


}

void LedManager::print(){
	 
	uint32_t mode = _ligne0.getMode();
	uint32_t color = _ligne0.getColor();
	uint32_t speed = _ligne0.getSpeed();
	uint32_t brightness = _ligne0.getBrightness();

	// Affiche le nom du mode
	Serial.println(violet("╭───────────────────────"));
	Serial.print(violet("│ Mode")+"       : ");
	Serial.println(jaune(_ligne0.getModeName(mode)));

	// Affiche la couleur en hexa
	Serial.print(violet("│ Couleur")+"    : ");
	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8)  & 0xFF;
	uint8_t b = color & 0xFF;
	char colorStr[7];
	snprintf(colorStr, sizeof(colorStr), "%02X%02X%02X", r, g, b);

	Serial.println(jaune("0x"+String(colorStr)));

	// Affiche la vitesse
	Serial.print(violet("│ Vitesse")+"    : ");
	Serial.println(jaune(String(speed)));

	// Affiche la luminosité
	Serial.print(violet("│ Luminosité")+" : ");
	Serial.println(jaune(String(brightness)));
	Serial.println(violet("╰───────────────────────"));
}

void LedManager::printMode(){

	Serial.println(bleu("╭────────── ")+ "Mode" +bleu(" ─────────────"));
	for (int i = 0; i < 80; ++i){
		Serial.println(bleu("│ ")+rouge(String(i))+ " => " + String(_ligne0.getModeName(i)));
	}
}

void LedManager::custome(){
	uint32_t colors[] = {RED, GREEN, BLUE};
	_ligne0.setSegment(0, 0,29,  FX_MODE_COMET, COLORS(RED, GREEN, BLUE), 1000, false);
	_ligne0.setSegment(1, 30, 59, FX_MODE_COMET,  COLORS(MAGENTA, PURPLE, ORANGE), 1000 ,true);
}

