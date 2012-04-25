#include "terminal/translator.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <locale.h>
#include <langinfo.h>
#include <iostream>

Translator::Translator(string from, string to)
	: ch_extra(0)
{
	cd = iconv_open(to.c_str(), from.c_str());
	if(cd == (iconv_t)-1)
	{
		if(errno == EINVAL)
			cerr << "warning: conversion from " << from << " to " << 
				to << " is not available." << endl;
		else
			perror("iconv_open");
		cd = 0;
	}
}


Translator::~Translator()
{
	if(cd)
		iconv_close(cd);
}


char 
Translator::Translate(char c)
{
	if(cd == 0)
		return c;

	char* inbuf = (char*)calloc(1, 1);
	inbuf[0] = c;
	char* wrptr = (char*)calloc(2, 1);
	size_t sz_i = 1, sz_o = 2;
	char* t = wrptr;
	size_t nconv = iconv(cd, &inbuf, &sz_i, &wrptr, &sz_o);

	cout << "nconv = " << nconv << endl;
	cout << "inbuf = " << (unsigned int)inbuf[0] << endl;
	cout << "sz_i = " << sz_i << endl;
	cout << "wrptr[0] = " << (unsigned int)wrptr[0] << endl;
	cout << "wrptr[1] = " << (unsigned int)wrptr[1] << endl;
	cout << "t[0] = " << (unsigned int)t[0] << endl;
	cout << "t[1] = " << (unsigned int)t[1] << endl;
	cout << "sz_o = " << sz_o << endl;
	cout << endl;

	if(nconv == (size_t)-1)
		perror("iconv");

	return c;
}


string
Translator::CurrentEncoding()
{
	char* loc = setlocale(LC_ALL, "");
	if(loc)
		return nl_langinfo(_NL_CTYPE_CODESET_NAME);
	else
	{
		cerr << "warning: current locale could not be identified." <<
			endl;
		return "";
	}
}


int main()
{
	cout << Translator::CurrentEncoding() << endl;
	Translator t("UTF-8", "latin1");
	t.Translate(0xc2);
	cout << t.Translate(0xa2) << endl;
	//cout << t.Translate('x') << endl;
}
