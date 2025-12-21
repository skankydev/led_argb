#include "function.h"
#include <WiFi.h>
#include <LittleFS.h>
#include <esp_ota_ops.h>
#include <esp_partition.h>

void setTime(time_t timestamp) {
	struct timeval tv;
	tv.tv_sec = timestamp;
	tv.tv_usec = 0;
	settimeofday(&tv, NULL);
}

String getTime() {
	time_t now = time(nullptr);
	struct tm* timeinfo = localtime(&now);
	char buffer[9];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
	return String(buffer);
}

String getDate() {
	time_t now = time(nullptr);
	struct tm* timeinfo = localtime(&now);
	char buffer[11];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
	return String(buffer);
}

String getDateTime(String format) {
	time_t now = time(nullptr);
	struct tm* timeinfo = localtime(&now);
	char buffer[64];
	strftime(buffer, sizeof(buffer), format.c_str(), timeinfo);
	return String(buffer);
}

uint32_t convertColor(String color){
	if(color.startsWith("#")){
		color = color.substring(1);
	}
	return strtol(color.c_str(), NULL, 16);
}

void println(String message,String cat){
	String toPrint = violet("["+getTime()+"]");
	toPrint += " "+cyan("["+cat+"]")+" ";
	toPrint += message;
	Serial.println(toPrint);
}

String strToLower(String text){
	std::transform(text.begin(), text.end(),text.begin(), ::tolower);
	return text; 
}
String strToUpper(String text){
	std::transform(text.begin(), text.end(),text.begin(), ::toupper);
	return text;
}


void info(String message,String cat){
	println("\033[36m"+message+"\033[0m",cat);
}

void warning(String message,String cat){
	println("\033[33m"+message+"\033[0m",cat);
}

void error(String message,String cat){
	println("\033[31m"+message+"\033[0m",cat);
}

void success(String message,String cat){
	println("\033[92m"+message+"\033[0m",cat);
}

String noir(String message){return "\033[30m"+message+"\033[0m";}
String rouge(String message){return "\033[31m"+message+"\033[0m";}
String vert(String message){return "\033[32m"+message+"\033[0m";}
String jaune(String message){return "\033[33m"+message+"\033[0m";}
String bleu(String message){return "\033[34m"+message+"\033[0m";}
String violet(String message){return "\033[35m"+message+"\033[0m";}
String cyan(String message){return "\033[36m"+message+"\033[0m";}
String blanc(String message){return "\033[37m"+message+"\033[0m";}

String grisClair(String message){return "\033[90m"+message+"\033[0m";}
String rougeVif(String message){return "\033[91m"+message+"\033[0m";}
String vertVif(String message){return "\033[92m"+message+"\033[0m";}
String jauneVif(String message){return "\033[93m"+message+"\033[0m";}
String bleuVif(String message){return "\033[94m"+message+"\033[0m";}
String violetVif(String message){return "\033[95m"+message+"\033[0m";}
String cyanVif(String message){return "\033[96m"+message+"\033[0m";}
String blancVif(String message){return "\033[97m"+message+"\033[0m";}

String toBin(uint16_t val) {
	String result = "";
	//Serial.print(valeur); Serial.print(F("\t--> "));
	for (int i = sizeof(uint16_t) * 8 - 1; i >= 0; i--) {
		result += (bitRead(val, i) == 0 ? "0" : "1");
		if (i%4 == 0){
			result += " ";
		};
	}
	return result;
}


String formatBytes(uint32_t bytes) {
	if (bytes < 1024) {
		return String(bytes) + String(" B");
	} else if (bytes < (1024 * 1024)) {
		return String(bytes / 1024.0) + String(" KB");
	} else if (bytes < (1024 * 1024 * 1024)) {
		return String(bytes / (1024.0 * 1024.0)) + String(" MB");
	} else {
		return String(bytes / (1024.0 * 1024.0 * 1024.0)) + String(" GB");
	}
}

String formatBytesPretty(uint32_t bytes) {
	if (bytes < 1024) {
		return String(bytes) + vert(" B");
	} else if (bytes < (1024 * 1024)) {
		return String(bytes / 1024.0) + vertVif(" KB");
	} else if (bytes < (1024 * 1024 * 1024)) {
		return String(bytes / (1024.0 * 1024.0)) + jaune(" MB");
	} else {
		return String(bytes / (1024.0 * 1024.0 * 1024.0)) + rouge(" GB");
	}
}

String getResetReason(int reason) {
	switch (reason) {
		case 1:  return String("Vbat power on reset"); break;
		case 3:  return String("Software reset digital core"); break;
		case 4:  return String("Legacy watch dog reset digital core"); break;
		case 5:  return String("Deep Sleep reset digital core"); break;
		case 6:  return String("Reset by SLC module, reset digital core"); break;
		case 7:  return String("Timer Group0 Watch dog reset digital core"); break;
		case 8:  return String("Timer Group1 Watch dog reset digital core"); break;
		case 9:  return String("RTC Watch dog Reset digital core"); break;
		case 10: return String("Instrusion tested to reset CPU"); break;
		case 11: return String("Time Group reset CPU"); break;
		case 12: return String("Software reset CPU"); break;
		case 13: return String("RTC Watch dog Reset CPU"); break;
		case 14: return String("for APP CPU, reset by PRO CPU"); break;
		case 15: return String("Reset when the vdd voltage is not stable"); break;
		case 16: return String("RTC Watch dog reset digital core and rtc module"); break;
		default: return String("NO_MEAN");
	}
}

void printSystemInfo() {
	println(vertVif("═══════════════════════════════════════════"), "SYSTEM");
	
	// Chip Info
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	
	println(cyan("Chip: ") + String(ESP.getChipModel()) + " Rev " + String(chip_info.revision), "SYSTEM");
	println(cyan("Cores: ") + String(chip_info.cores), "SYSTEM");
	println(cyan("Frequency: ") + String(ESP.getCpuFreqMHz()) + " MHz", "SYSTEM");
	
	// Memory Info
	println(vertVif("───────────────────────────────────────────"), "SYSTEM");
	uint32_t freeHeap = ESP.getFreeHeap();
	uint32_t totalHeap = ESP.getHeapSize();
	uint32_t usedHeap = totalHeap - freeHeap;
	float heapPercent = (usedHeap * 100.0) / totalHeap;
	
	println(cyan("Heap Total: ") + formatBytesPretty(totalHeap), "MEMORY");
	println(cyan("Heap Used: ") + formatBytesPretty(usedHeap) + String(" (") + String(heapPercent, 1) + "%)", "MEMORY");
	println(cyan("Heap Free: ") + formatBytesPretty(freeHeap), "MEMORY");
	
	// PSRAM si disponible
	if (psramFound()) {
		uint32_t freePsram = ESP.getFreePsram();
		uint32_t totalPsram = ESP.getPsramSize();
		uint32_t usedPsram = totalPsram - freePsram;
		float psramPercent = (usedPsram * 100.0) / totalPsram;
		
		println(cyan("PSRAM Total: ") + formatBytesPretty(totalPsram), "MEMORY");
		println(cyan("PSRAM Used: ") + formatBytesPretty(usedPsram) + String(" (") + String(psramPercent, 1) + "%)", "MEMORY");
		println(cyan("PSRAM Free: ") + formatBytesPretty(freePsram), "MEMORY");
	}
	
	// Flash Info
	println(vertVif("───────────────────────────────────────────"), "SYSTEM");
	uint32_t flashSize = ESP.getFlashChipSize();
	println(cyan("Flash Size: ") + formatBytesPretty(flashSize), "FLASH");
	println(cyan("Flash Speed: ") + String(ESP.getFlashChipSpeed() / 1000000) + " MHz", "FLASH");
	
	// Sketch Info
	uint32_t sketchSize = ESP.getSketchSize();
	uint32_t sketchFree = ESP.getFreeSketchSpace();
	uint32_t sketchTotal = sketchSize + sketchFree;
	float sketchPercent = (sketchSize * 100.0) / sketchTotal;
	
	println(cyan("Sketch Used: ") + formatBytesPretty(sketchSize) + String(" (") + String(sketchPercent, 1) + "%)", "FLASH");
	println(cyan("Sketch Free: ") + formatBytesPretty(sketchFree), "FLASH");
	
	// Reset Info
	println(vertVif("───────────────────────────────────────────"), "SYSTEM");
	println(cyan("Reset Reason: ") + getResetReason(esp_reset_reason()), "SYSTEM");
	println(cyan("Uptime: ") + String(millis() / 1000) + " seconds", "SYSTEM");
	
	// MAC Address
	println(cyan("MAC Address: ") + WiFi.macAddress(), "SYSTEM");
	
	println(vertVif("═══════════════════════════════════════════"), "LITTLEFS");
	
	uint32_t totalBytes = LittleFS.totalBytes();
	uint32_t usedBytes = LittleFS.usedBytes();
	uint32_t freeBytes = totalBytes - usedBytes;
	float fsPercent = (usedBytes * 100.0) / totalBytes;
	
	println(cyan("LittleFS Total: ") + formatBytesPretty(totalBytes), "FS");
	println(cyan("LittleFS Used: ") + formatBytesPretty(usedBytes) + String(" (") + String(fsPercent, 1) + "%)", "FS");
	println(cyan("LittleFS Free: ") + formatBytesPretty(freeBytes), "FS");
	
}

void printAllPartitions() {
	println(vertVif("═══════════════════════════════════════════"), "PARTITIONS");
	
	esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
	
	while (it != NULL) {
		const esp_partition_t* part = esp_partition_get(it);
		
		String type;
		switch(part->type) {
			case ESP_PARTITION_TYPE_APP:  type = "APP"; break;
			case ESP_PARTITION_TYPE_DATA: type = "DATA"; break;
			default: type = "UNKNOWN";
		}
		
		String subtype;
		if (part->type == ESP_PARTITION_TYPE_APP) {
			switch(part->subtype) {
				case ESP_PARTITION_SUBTYPE_APP_FACTORY: subtype = "factory"; break;
				case ESP_PARTITION_SUBTYPE_APP_OTA_0: subtype = "ota_0"; break;
				case ESP_PARTITION_SUBTYPE_APP_OTA_1: subtype = "ota_1"; break;
				default: subtype = "ota_" + String(part->subtype - ESP_PARTITION_SUBTYPE_APP_OTA_0);
			}
		} else if (part->type == ESP_PARTITION_TYPE_DATA) {
			switch(part->subtype) {
				case ESP_PARTITION_SUBTYPE_DATA_OTA: subtype = "ota"; break;
				case ESP_PARTITION_SUBTYPE_DATA_NVS: subtype = "nvs"; break;
				case ESP_PARTITION_SUBTYPE_DATA_SPIFFS: subtype = "spiffs"; break;
				case ESP_PARTITION_SUBTYPE_DATA_FAT: subtype = "fat"; break;
				default: subtype = "data";
			}
		}
		
		println(
			cyan(String(part->label).length() > 0 ? part->label : "unnamed") + 
			" [" + type + "/" + subtype + "] " +
			"@ 0x" + String(part->address, HEX) + " - " +
			formatBytesPretty(part->size),
			"PARTITION"
		);
		
		it = esp_partition_next(it);
	}
	
	esp_partition_iterator_release(it);
	println(vertVif("═══════════════════════════════════════════"), "PARTITIONS");
}