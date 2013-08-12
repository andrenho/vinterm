#include "graphic/clipboard.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>

Clipboard::Clipboard()
{
}


Clipboard::~Clipboard()
{
}


string 
Clipboard::Read()
{
	Display* dpy = XOpenDisplay(NULL);
	string s;

	Window Sown = XGetSelectionOwner(dpy, XA_PRIMARY);
	if(Sown != None) 
	{
		Atom type;
		unsigned long len, bytes_left;
		int format, dummy;
		unsigned char* data;

		XConvertSelection(dpy, XA_PRIMARY, XA_STRING, None, Sown, CurrentTime);
		XFlush(dpy);
		XGetWindowProperty(dpy, Sown, XA_STRING, 0, 0, 0, AnyPropertyType,
				&type, &format, &len, &bytes_left, &data);
		if(bytes_left > 0)
		{
			int result = XGetWindowProperty(dpy, Sown, XA_STRING, 0,
					bytes_left, 0, AnyPropertyType,
					&type, &format, &len, &dummy, &data);
			if(result == Success)
				s = string(data);
			XFree(data);
		}
	}

	return s;
}
