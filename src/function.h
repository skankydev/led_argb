#pragma once

#include "setting.h"
#include <time.h>

void setTime(time_t timestamp);
String getTime();
String getDate();
String getDateTime(String format = "%Y-%m-%d %H:%M:%S");

void println(String message,String cat = "SYS");

String strToLower(String text);
String strToUpper(String text);



uint32_t convertColor(String color);

void info(String message,String cat = "SYS");
void warning(String message,String cat = "SYS");
void error(String message,String cat = "SYS");
void success(String message,String cat = "SYS");

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
void printSystemInfo();
void printAllPartitions();