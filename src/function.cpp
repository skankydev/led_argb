#include "function.h"

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.


String getDate(){
	String date = getDateString();
	String time = getTimeString();
	return date + " " + time;
}

void printDate(){
	String date = getDateString();
	String time = getTimeString();
	Serial.println(date + " " + time);
}
String getTimeString(){
	time_t tm = now();
	int h,m,s;
	String hs,ms,ss;
	h = hour(tm);
	m = minute(tm);
	s = second(tm);

	hs = String(h);
	ms = String(m);
	ss = String(s);

	if(hs.length() == 1){
		hs = "0"+hs;
	}
	if(ms.length() == 1){
		ms = "0"+ms;
	}
	if(ss.length() == 1){
		ss = "0"+ss;
	}

	return hs+":"+ms+":"+ss;
}

String getDateString(){
	time_t tm = now();
	int d, m, y;
	String ds,ms,ys;
	d = day(tm);
	m = month(tm);
	y = year(tm);

	ds = String(d);
	ms = String(m);
	ys = String(y);

	if(ds.length() == 1){
		ds = "0"+ds;
	}
	if(ms.length() == 1){
		ms = "0"+ms;
	}

	return ds+"/"+ms+"/"+ys;
}


String getDateIso(){
	String date = "";
	time_t tm = now();
	date += String(year(tm));
	String tmp;
	tmp = String(month(tm));
	if(tmp.length() == 1){
		tmp = "0"+tmp;
	}
	date += "-"+tmp;
	tmp = String(day(tm));
	if(tmp.length() == 1){
		tmp = "0"+tmp;
	}
	date += "-"+tmp;

	tmp = String(hour(tm));
	if(tmp.length() == 1){
		tmp = "0"+tmp;
	}
	date += "T"+tmp;

	tmp = String(minute(tm));
	if(tmp.length() == 1){
		tmp = "0"+tmp;
	}
	date += "h"+tmp;

	tmp = String(second(tm));
	if(tmp.length() == 1){
		tmp = "0"+tmp;
	}
	date += "m"+tmp;

	return date;

}

String strToLower(String text){
	transform(text.begin(), text.end(),text.begin(), ::tolower);
	return text; 
}
String strToUpper(String text){
	transform(text.begin(), text.end(),text.begin(), ::toupper);
	return text;
}


void info(String message){
	Serial.println("\033[36m"+message+"\033[0m");
}

void warning(String message){
	Serial.println("\033[33m"+message+"\033[0m");
}

void error(String message){
	Serial.println("\033[31m"+message+"\033[0m");
}

void success(String message){
	Serial.println("\033[92m"+message+"\033[0m");
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

