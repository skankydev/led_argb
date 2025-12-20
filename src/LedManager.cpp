#include "LedManager.h"

LedManager *LedManager::instance = nullptr;

LedManager *LedManager::getInstance()
{
	if (!instance) {
		instance = new LedManager;
	}
	return instance;
}

LedManager::LedManager() : _ligne1(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800) {


	_ligne1.init();	
	this->setDefault();
	_ligne1.start();

}

void LedManager::step(){
	_ligne1.service();

	if(_autoMode) {
		unsigned long now = millis();
		if(now - _lastChange > _autoDelay) {
			this->setNextEffect();
			_lastChange = now;
		}
	}
}


void LedManager::toggleAutoMode() {
	_autoMode = !_autoMode;
	if(_autoMode) {
		_lastChange = millis();  // Reset le timer quand on active
		success("Auto mode activé");
	} else {
		warning("Auto mode désactivé");
	}
}

bool LedManager::getAutoMode() {
	return _autoMode;
}

void LedManager::setAutoDelay(uint32_t delayMs) {
	_autoDelay = delayMs;
	Serial.println(bleu("Auto delay")+" : " + String(delayMs) + "ms");
}

/**
 * Change l'effet
 */
void LedManager::setNextEffect(String target) {
	uint32_t mode = _ligne1.getMode();
	mode += 1;
	if(mode > 71){
		mode = 0;
	}
	_ligne1.setMode(mode);
	Serial.println(bleu("Mode") + " : " + rouge(String(mode))+ " -> " + jaune(String(_ligne1.getModeName(mode))));
}

/**
 * Change l'effet
 */
void LedManager::setEffect(uint8_t mode, String target) {
	_ligne1.setMode(mode);
	Serial.println(bleu("Mode") + " : " + rouge(String(mode))+ " -> " + jaune(String(_ligne1.getModeName(mode))));
}


/**
 * Change la couleur
 */
void LedManager::setColor(uint32_t color, String target) {
	_ligne1.setColor(color);
}

/**
 * Change la vitesse
 */
void LedManager::setSpeed(uint16_t speed, String target) {
	_ligne1.setSpeed(speed);
}

/**
 * Change la luminosité
 */
void LedManager::setBrightness(uint8_t brightness, String target) {
	Serial.println(jaune("Brightness")+" : "+String(brightness));
	_ligne1.setBrightness(brightness);
}

void LedManager::setSegments(JsonArray segments, String target){
	int id = 0;

	_ligne1.resetSegments();
	_ligne1.resetSegmentRuntimes();

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

		_ligne1.setSegment(id, first, last, effect, color, speed, reverse);
		id++;
	}
}

uint8_t LedManager::getBrightness(String target) {
	return _ligne1.getBrightness();
}


void LedManager::run(){
	unsigned long start = millis();
	while(millis() - start < 10000) {
		_ligne1.service();
	}
	this->setDefault();
}

void LedManager::setDefault(){
	_autoMode = false;
	_lastChange = 0;
	_autoDelay = 5000; 

	// Effacer tous les segments
	_ligne1.resetSegments();
	_ligne1.resetSegmentRuntimes();
	
	// Recréer un nouveau segment sur tout le bandeau	
	_ligne1.setSegment(0, 0, NUM_LEDS-1, FX_MODE_STATIC, 0x0000FF, 1000, false);	

	_ligne1.setBrightness(30);
	_ligne1.setColor(0x0000FF);
	_ligne1.setSpeed(1000);
	_ligne1.setMode(FX_MODE_STATIC);


}



/**
 * Effet Noël - Twinkle Random rouge/vert
 */
void LedManager::noel() {
	_ligne1.setMode(FX_MODE_TWINKLE_RANDOM);
	_ligne1.setColor(RED);
	_ligne1.setSpeed(800);
	success("Mode Noel - Twinkle");
	// Ajoute du vert en couleur secondaire
	uint32_t colors[] = {RED, GREEN, RED};
	_ligne1.setColors(0, colors);
	
	this->run();
}

/**
 * Effet Noël alternatif - Fire Flicker rouge/orange
 */
void LedManager::noelBis() {
	_ligne1.setMode(FX_MODE_FIRE_FLICKER);
	_ligne1.setColor(RED);
	_ligne1.setSpeed(500);
	success("Mode Noel Bis - Fire Flicker");
	this->run();
}

void LedManager::blink() {
	_ligne1.setColor(0x00FF00);
	_ligne1.setMode(FX_MODE_BLINK);
	_ligne1.setSpeed(500);
	info("Blink");
	this->run();
}


void LedManager::rainbow(){
	_ligne1.setMode(FX_MODE_RAINBOW_CYCLE);
	_ligne1.setSpeed(1000);
	info("Rainbow Cycle");
	this->run();
}

void LedManager::scanner(){
	_ligne1.setColor(0xFF0000);
	_ligne1.setMode(FX_MODE_LARSON_SCANNER);
	_ligne1.setSpeed(800);
	info("Larson Scanner");
	this->run();
}


void LedManager::theaterChaseRainbow(){
	_ligne1.setMode(FX_MODE_THEATER_CHASE_RAINBOW);
	_ligne1.setSpeed(1000);
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
	 
	uint32_t mode = _ligne1.getMode();
	uint32_t color = _ligne1.getColor();
	uint32_t speed = _ligne1.getSpeed();
	uint32_t brightness = _ligne1.getBrightness();

	// Affiche le nom du mode
	Serial.println(violet("╭───────────────────────"));
	Serial.print(violet("│ Mode")+"       : ");
	Serial.println(jaune(_ligne1.getModeName(mode)));

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
		Serial.println(bleu("│ ")+rouge(String(i))+ " => " + String(_ligne1.getModeName(i)));
	}
}

void LedManager::custome(){
	uint32_t colors[] = {RED, GREEN, BLUE};
	_ligne1.setSegment(0, 0,29,  FX_MODE_COMET, COLORS(RED, GREEN, BLUE), 1000, false);
	_ligne1.setSegment(1, 30, 59, FX_MODE_COMET,  COLORS(MAGENTA, PURPLE, ORANGE), 1000 ,true);
}