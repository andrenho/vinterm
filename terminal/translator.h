#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <iconv.h>
#include <string>
using namespace std;

class Translator
{
public:
	Translator(string from, string to);
	~Translator();
	char Translate(char c);

	static string CurrentEncoding();

private:
	iconv_t cd;
	char ch_extra;
};

#endif
