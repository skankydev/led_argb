#ifndef DEF_FUNCTION
#define DEF_FUNCTION

#include "setting.h"


void printDate();
String getTimeString();
String getDateString();
String getDateIso();

String strToLower(String text);
String strToUpper(String text);

uint32_t convertColor(String color);

void text(String message);
void info(String message);
void warning(String message);
void error(String message);
void success(String message);

String noir(String message);
String rouge(String message);
String vert(String message);
String jaune(String message);
String bleu(String message);
String violet(String message);
String cyan(String message);
String blanc(String message);

String grisClair(String message);
String rougeVif(String message);
String vertVif(String message);
String jauneVif(String message);
String bleuVif(String message);
String violetVif(String message);
String cyanVif(String message);
String blancVif(String message);

String toBin(uint16_t val);

String formatBytes(uint32_t bytes);
String formatBytesPretty(uint32_t bytes);
String getResetReason(int reason);

#endif
