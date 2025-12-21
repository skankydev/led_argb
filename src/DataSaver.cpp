#include "DataSaver.h"


/**
 * Sauvegarde d un fichier ( avec copy )
 * @param fileName nom de fichier ex:'fichier.txt'
 * @param content  le contenu
 * @param copy     si on veux une copy ( par defaut oui)
 */
void DataSaver::save(String fileName,String content) {
	File file = LittleFS.open("/"+fileName, FILE_WRITE);
	file.print(content);
	file.close();
}

/**
 * lire le contenu d un fichier 
 * @param  fileName nom de fichier ex:'fichier.txt'
 * @return          le contenu du fichier
 */
String DataSaver::read(String fileName){
	if (!LittleFS.exists("/"+fileName)) {
		error("le fichier existe pas : "+fileName,"LFS");
		return String("");
	}
	File file = LittleFS.open("/"+fileName, FILE_READ);
	String content = file.readString();
	file.close();
	return content;
}

/**
 * Ajoute du contenu en fin de fichier 
 * @param fileName nom de fichier ex:'fichier.txt'
 * @param content  le contenu
 */
void DataSaver::append(String name, String content){
	File file = LittleFS.open("/"+name, FILE_APPEND);
	file.println(content);
	file.close();
}

/**
 * suprime un fichier
 * @param fileName nom de fichier ex:'fichier.txt'
 */
void DataSaver::clear(String fileName) {
	LittleFS.remove("/"+fileName);
}


/**
 * Crée un docier 
 * @param  dirName Nom du docier 
 * @return         créé oui ou non
 */
bool DataSaver::mkdir(String dirName) {
	if (!LittleFS.exists("/"+dirName)) {
		return LittleFS.mkdir("/"+dirName);
	}
	return false;
}

/**
 * suprime un docier et tout sont contenu
 * @param dirName Nom du docier 
 */
void DataSaver::deleteDir(String dirName){
	String path = "/"+dirName+"/";
	File root = LittleFS.open(path);
	File file = root.openNextFile();
	while (file) {
		String name = file.name();
		info(name);
		file.close();
		if (!LittleFS.remove(path + name)) {
			error("Erreur lors de la suppression du fichier " + String(path  + file.name()),"LFS");
		} else {
			success("Fichier supprimé : " + String(path + file.name()),"LFS");
		}
		file = root.openNextFile();
	}
	LittleFS.remove(path);
}

/**
 * liste tout les fichier et docier d un chemin de fasson recursive
 * @param path   le chemin  "/"
 * @param prefix utiliser pour faire du joli
 */
void DataSaver::listAll(String path,String prefix){
	File root = LittleFS.open(path);
	if (!root) {
		error("Erreur lors de l'ouverture du répertoire racine","LFS");
		return;
	}
	File file = root.openNextFile();
	while (file) {
		if (!file.isDirectory()) {
			uint32_t fileSize = file.size();

			Serial.println(violetVif(prefix+file.name())+" "+formatBytes(fileSize));
		}else{
			warning(prefix+file.name() );
			listAll(path + file.name() + "/",prefix+"|- ");
		}
		file.close();
		file = root.openNextFile();
	}
	root.close();
}

/**
 * appelle liste all depui la racine
 */
void DataSaver::doListAll(){
	listAll("/");
	for (int i = 0; i < 10; ++i){
		delay(10);
		Serial.print(vertVif("_"));
		delay(10);
		Serial.print(bleu("_"));
	}
	Serial.println(" ");
	
}

/**
 * Supprime tout le contenu de LittleFS
 * @param path chemin de fichier
 */
void DataSaver::format(String path) {
	File root = LittleFS.open(path);
	if (!root) {
		error("Erreur lors de l'ouverture du répertoire racine","LFS");
		return;
	}

	// Parcourir tous les fichiers
	File file = root.openNextFile();
	while (file) {
		if (!file.isDirectory()) {
			String name = file.name();
			info(name);
			file.close();
			// Supprimer le fichier
			if (!LittleFS.remove(path + name)) {
				error("Erreur lors de la suppression du fichier " + String(path  +name),"LFS");
			} else {
				success("Fichier supprimé : " + String(path + name),"LFS");
			}
		}else{
			format(path  + file.name()+ "/");
			String name = file.name();
			file.close();
			LittleFS.rmdir(path  + name);
		}
		file = root.openNextFile();
	}

	// Fermez le répertoire
	root.close();
}


/**
 * appelle format depui la racine
 */
void DataSaver::doFormat(){
	format("/");
	info("j ais fini");
	for (int i = 0; i < 10; ++i){
		delay(100);
		Serial.print(vertVif("_"));
		delay(100);
		Serial.print(bleuVif("_"));
	}
	Serial.println(" ");
	
}
