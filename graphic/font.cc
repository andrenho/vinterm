#include "graphic/font.h"

#include "options.h"
#include "graphic/fontbmp.h"

Font* 
Font::LoadFont()
{
	return new FontBMP("data/850.bmp", 9, 14, 32, "ibm850");
}
