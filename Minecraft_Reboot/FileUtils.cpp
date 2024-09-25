#include "FileUtils.h"

string FileUtils::readFile(const char* file) {
	ifstream inStream;
	stringstream ss;

	inStream.open(file);

	if (inStream.fail()) {
		cout << "Failed to read file" << endl;
		
		// Exit the program
		exit(-1);
	}

	// fetch content of the file to stringstream
	ss << inStream.rdbuf();

	inStream.close();
	
	return ss.str();
}