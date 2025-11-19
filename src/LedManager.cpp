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
	_ligne1.setBrightness(80);
	_ligne1.setSpeed(1000);
	_ligne1.setColor(0x0000FF);  // Bleu
	_ligne1.setMode(FX_MODE_STATIC);
	_ligne1.start();

}

void LedManager::step(){
	_ligne1.service();
}

/**
 * Change l'effet
 */
void LedManager::setEffect(uint8_t mode, String target) {
	_ligne1.setMode(mode);
	info("Mode: " + String(_ligne1.getModeName(mode)));
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
	_ligne1.setBrightness(brightness);
}


void LedManager::run(){
	unsigned long start = millis();
	while(millis() - start < 10000) {
		_ligne1.service();
	}
	this->setDefault();
}

void LedManager::setDefault(){
	_ligne1.setBrightness(80);
	_ligne1.setColor(0x0000FF);
	_ligne1.setSpeed(1000);
	_ligne1.setMode(FX_MODE_STATIC);
	_ligne1.service();
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
	Serial.println(bleu("╭───────────────────────"));
	Serial.print(bleu("│ Mode")+"       : ");
	Serial.println(jaune(_ligne1.getModeName(mode)));

	// Affiche la couleur en hexa
	Serial.print(bleu("│ Couleur")+"    : ");
	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8)  & 0xFF;
	uint8_t b = color & 0xFF;
	char colorStr[7];
	snprintf(colorStr, sizeof(colorStr), "%02X%02X%02X", r, g, b);

	Serial.println(jaune("0x"+String(colorStr)));

	// Affiche la vitesse
	Serial.print(bleu("│ Vitesse")+"    : ");
	Serial.println(jaune(String(speed)));

	// Affiche la luminosité
	Serial.print(bleu("│ Luminosité")+" : ");
	Serial.println(jaune(String(brightness)));
	Serial.println(bleu("╰───────────────────────"));
}
