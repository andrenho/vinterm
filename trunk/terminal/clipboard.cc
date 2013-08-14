#include "terminal/clipboard.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include "SDL_syswm.h"

void 
Clipboard::ConnectToWM()
{
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	if(SDL_GetWMInfo(&info) != 1)
	{
		fprintf(stderr, "It wasn't possible to retrieve WM info.\n");
		exit(-1);
	}

	display = (void*)info.info.x11.display;
	window = info.info.x11.window;
}


string 
Clipboard::Read() const
{
	Display* dpy = (Display*)display;
	string s;

	for(int i=0; i<2; i++)
	{
		XFlush(dpy);
		Window Sown = XGetSelectionOwner(dpy, XA_PRIMARY);
		if(Sown != None) 
		{
			Atom type;
			unsigned long len, bytes_left, dummy;
			int format;
			unsigned char* data;

			XConvertSelection(dpy, XA_PRIMARY, XA_STRING, None, Sown, 
					CurrentTime);
			XFlush(dpy);
			XGetWindowProperty(dpy, Sown, XA_STRING, 0, 0, 0, 
					AnyPropertyType, &type, &format, &len, 
					&bytes_left, &data);
			if(bytes_left > 0)
			{
				int result = XGetWindowProperty(dpy, Sown, XA_STRING, 0,
						bytes_left, 0, AnyPropertyType,
						&type, &format, &len, &dummy, &data);
				if(result == Success)
					s = string((char*)data);
				XFree(data);
			}
		}
	}

	return s;
}


void
Clipboard::Store(string s)
{
	str_clipboard = s;

	Display* dpy = (Display*)display;
	XSetSelectionOwner(dpy, XA_PRIMARY, window, CurrentTime);
	XFlush(dpy);
	if(XGetSelectionOwner(dpy, XA_PRIMARY) != window)
		fprintf(stderr, "warning: we didn't get the selection\n");
}


void
Clipboard::Respond() const
{
	Display* dpy = (Display*)display;
	XEvent e, respond;

	if(XCheckTypedEvent(dpy, SelectionRequest, &e))
	{
		if(XGetSelectionOwner(dpy, XA_PRIMARY) != window)
			fprintf(stderr, "warning: we didn't get the selection\n");

		// somebody wants our data
		XSelectionRequestEvent *req = &(e.xselectionrequest);

		if(req->target == XA_STRING 
		|| !strcmp(XGetAtomName(dpy, req->target), "TEXT")
		|| !strcmp(XGetAtomName(dpy, req->target), "UTF8_STRING"))
		{
			XChangeProperty(dpy, req->requestor, req->property,
					XA_STRING, 8, PropModeReplace,
					(unsigned char*)(str_clipboard.c_str()),
					str_clipboard.length());
			respond.xselection.property = req->property;
		}
		else
		{
			fprintf(stderr, "warning: invalid target %s\n",
					XGetAtomName(dpy, req->target));
			respond.xselection.property = None;
		}
		respond.xselection.type = SelectionNotify;
		respond.xselection.display = req->display;
		respond.xselection.requestor = req->requestor;
		respond.xselection.selection = req->selection;
		respond.xselection.target = req->target;
		respond.xselection.time = req->time;
		XSendEvent(dpy, req->requestor, 0, 0, &respond);
		XFlush(dpy);
	}

	// TODO - check for selection lost
}
