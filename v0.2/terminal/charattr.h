#ifndef CHARATTR_H
#define CHARATTR_H

class Attribute {
public:
	Attribute() 
		: Reverse(false), Underline(false), Highlight(false), 
		  Blink(false) { }
	bool Reverse    : 1;
	bool Underline  : 1;
	bool Highlight  : 1;
	bool Blink      : 1;

	operator long int() const 
	{ 
		return (Reverse << 3) + (Underline << 2) + (Highlight << 1) + 
			Blink; 
	}
	
	const int Max() const { return 0xf; }
};

typedef struct Char {
	char Ch;
	Attribute Attr;
	Char() : Ch(' ') { }
} Char;

#endif
