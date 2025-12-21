#pragma once

#include "setting.h"
#include <LittleFS.h>

class DataSaver{

	private:
		
	public:
		
		static void save(String fileName,String content);
		static String read(String fileName);
		static void append(String name, String content);

		static void clear(String fileName);
		static bool mkdir(String dirName);
		static void deleteDir(String dirName);

		static void listAll(String path,String prefix = "");
		static void doListAll();

		static void format(String path);
		static void doFormat();
		
};
