#ifndef CHARATTR_H
#define CHARATTR_H

class Attribute {
public:
	Attribute() 
		: Reverse(false), Underline(false), Highlight(false), 
		  Blink(false), Dim(false) { }
	bool Reverse    : 1;
	bool Underline  : 1;
	bool Highlight  : 1;
	bool Blink      : 1;
	bool Dim        : 1;

	operator long int() const 
	{ 
		return (Reverse << 3) + (Underline << 2) + (Highlight << 1) + 
		       Blink + (Dim << 5);
	}
	
	const int Max() const { return 0x1f; }
};

typedef struct Char {
	char Ch;
	Attribute Attr;
	Char() : Ch(' ') { }
} Char;

#endif