/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2Background.h
 *
 * Copyright (c) 2005, FixXxeR (avelar@gmail.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *      
 *      Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *      
 *      Neither the name of the <ORGANIZATION> nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * (See the included file COPYING / BSD )
 */

#ifndef XIMLIB2BACKGROUND_CLASS
#define XIMLIB2BACKGROUND_CLASS

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <Imlib2.h>
#include <X11/Xatom.h>

#include "DesktopContainer.h"
#include "DesktopConfig.h"
#include "XDesktopContainer.h"
#include "Timer.h"

class XImlib2Background: public TimerControl{
    public:
	Imlib_Image spareRoot;
	Pixmap pixmap;
	 
    private:
	vector<char*> data[2];
	vector<char*> *save, *show; 
	int delay, mode;
	time_t srctime;
	string images;
	string directory;
    
    protected:
        AbstractConfig * config;
        AbstractContainer * container;
	Colormap cmap;
	Visual * visual;
	bool OneShot;
   
    public:
	XImlib2Background(AbstractContainer * c, AbstractConfig * con);
	~XImlib2Background();
	virtual void SetMultiImage(Imlib_Image image);
	virtual void SetOneImage();
        void SetDelay( int d );
	void Refresh(Pixmap currentPixmap);
	Window createWindow();
	void InitSpareRoot(Window win_context);
	Pixmap GetRootPixmap(Atom id);
	Imlib_Image createCropImage(int x, int y, int src_width, int src_height, int dest_width, int dest_height);
	
	bool IsOneShot() { return OneShot; }
	void OnTime();
	
	void esetroot_pixmap_property(Window root_window, Pixmap p, GC gc, int w, int h);
			
	//static Pixmap make_root_pixmap (int width, int height);
	/* Create a persistant pixmap. We create a separate display
	* and set the closedown mode on it to RetainPermanent
	*/
	static Pixmap make_root_pixmap (Display *dpy, int width, int height)
	{
		Pixmap result;
		Display *ndisplay;

		XFlush(dpy);

		ndisplay = XOpenDisplay (NULL);
		XSetCloseDownMode (ndisplay, RetainPermanent);
	
		int screen = DefaultScreen(ndisplay);
		int depth =  DefaultDepth(ndisplay,screen);

		result = XCreatePixmap (ndisplay,
					DefaultRootWindow (ndisplay),
					width, height, depth);

		XCloseDisplay (ndisplay);

		return result;
	}
	
	/* Set the root pixmap, and properties pointing to it. We
	* do this atomically with XGrabServer to make sure that
	* we won't leak the pixmap if somebody else it setting
	* it at the same time. (This assumes that they follow the
	* same conventions we do
	*/
	static void set_root_pixmap (Display *dpy, Window win, Pixmap pixmap)
	{
		Atom type;
		unsigned long nitems, bytes_after;
		int format;
		unsigned char *data_esetroot;
		int result;
		
		Atom prop_root = XInternAtom(dpy, "_XROOTPMAP_ID", false);
		Atom prop_esetroot = XInternAtom(dpy, "ESETROOT_PMAP_ID", false);

		XGrabServer (dpy);

		result = XGetWindowProperty (dpy, win,
					     prop_esetroot,
					     0L, 1L, False, XA_PIXMAP,
					     &type, &format, &nitems, &bytes_after,
					     &data_esetroot);

		if (result == Success && type == XA_PIXMAP && format == 32 && nitems == 1) {
			               XKillClient(dpy, *(Pixmap*)data_esetroot);
				       //cout << "XKillClient -> " << endl;
		}

		if (data_esetroot != NULL) {
			XFree (data_esetroot);
		}

		if (pixmap != None) {
			XChangeProperty (dpy, win,
				prop_esetroot, 
				XA_PIXMAP, 32, PropModeReplace,
				(unsigned char *) &pixmap, 1);
			XChangeProperty (dpy, win,
				prop_root, 
				XA_PIXMAP, 32, PropModeReplace,
				 (unsigned char *) &pixmap, 1);
			XSetWindowBackgroundPixmap (dpy, win, pixmap);
		} else {
			XDeleteProperty (dpy, win, prop_esetroot);
			XDeleteProperty (dpy, win,prop_root);
		}

		XClearWindow (dpy, win);
		XUngrabServer (dpy);
		XFlush(dpy);
	}
	
    private:
	void Init();
	void SwapData();
	void FreeData();
	void ScanImages();
	void ChangeImage();
};

#endif
