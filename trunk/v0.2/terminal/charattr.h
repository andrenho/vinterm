#ifndef CHARATTR_H
#define CHARATTR_H

class Attribute {
public:
	Attribute() 
		: Reverse(false), Underline(false), Highlight(false), 
		  Blink(false), Cursor(false) { }
	bool Reverse    : 1;
	bool Underline  : 1;
	bool Highlight  : 1;
	bool Blink      : 1;
	bool Cursor     : 1;

	operator long int() const 
	{ 
		return (Reverse << 4) + (Underline << 3) + (Highlight << 2) + 
		       (Blink << 1) + Cursor; 
	}
	
	const int Max() const { return 0x1f; }
};

typedef struct Char {
	char Ch;
	Attribute Attr;
	Char() : Ch(' ') { }
} Char;

#endif
