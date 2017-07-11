#include "floatfile.h"
#pragma warning (disable : 4996)
ORION_NAMESPACE_BEGIN
bool readFloatFile(const char * filename, std::vector<Float>* values)
{
	FILE *f = fopen(filename, "r");
	if (!f) {
		CHECK_INFO(false, std::string("Unable to open file ") + filename);
		return false;
	}

	int c;
	bool inNumber = false;
	char curNumber[32];
	int curNumberPos = 0;
	int lineNumber = 1;
	while ((c = getc(f)) != EOF) {
		if (c == '\n') ++lineNumber;
		if (inNumber) {
			if (isdigit(c) || c == '.' || c == 'e' || c == '-' || c == '+')
				curNumber[curNumberPos++] = c;
			else {
				curNumber[curNumberPos++] = '\0';
				values->push_back(atof(curNumber));
				inNumber = false;
				curNumberPos = 0;
			}
		}
		else {
			if (isdigit(c) || c == '.' || c == '-' || c == '+') {
				inNumber = true;
				curNumber[curNumberPos++] = c;
			}
			else if (c == '#') {
				while ((c = getc(f)) != '\n' && c != EOF)
					;
				++lineNumber;
			}
			else if (!isspace(c)) {
				CHECK_INFO(false, "Unexpected text found at line " << lineNumber << " of float file " << filename);
			}
		}
	}
	fclose(f);
	return true;
}
ORION_NAMESPACE_END
