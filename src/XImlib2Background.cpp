
/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2Background.cpp
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

#include "Database.h"
#include "XImlib2Background.h"
#include <X11/Xatom.h>

XImlib2Background::XImlib2Background(AbstractContainer * c, AbstractConfig * con): TimerControl(), 
                                                    container(c), config(con),delay(0), mode(0),  save(NULL), show(NULL),
						    srctime(0), images(""), directory(""), OneShot(false)
{
     cmap = imlib_context_get_colormap();
     visual = imlib_context_get_visual();
     
     DesktopConfig * dConfig =
		    dynamic_cast<DesktopConfig *>(config);
    
     if( !show) {
		 show = &data[0];
	     	 save = &data[1];
     }
	
     int d = dConfig->getDelayBackground();
     directory = dConfig->getSourceBackground();

	
     if(d == 0 || directory == "None"){
	     OneShot = true;
     }
     
     srand( (unsigned)time(0) );
     
     if(OneShot){
		SetOneImage();
     }else{
		ScanImages();
		SetDelay( d );
     }
	
}

XImlib2Background::~XImlib2Background()
{
	if(spareRoot){	
		imlib_context_set_image(spareRoot);
		imlib_free_image();
	}
	FreeData();
}


void XImlib2Background::esetroot_pixmap_property(Window root_window, Pixmap p, GC gc , int w, int h)
{
	Pixmap esetroot_pixmap = None;
	
	XDesktopContainer * xContainer =
			dynamic_cast<XDesktopContainer *>(container);
	
	Display* display = xContainer->getDisplay();
	int screen = DefaultScreen(display);

	if (!p)
		set_root_pixmap(display,  root_window, None);
  
	if (root_window)
	{	
		esetroot_pixmap = make_root_pixmap(display, w, h);
		XCopyArea(display, p , esetroot_pixmap,  gc , 0, 0, w, h, 0, 0);
		set_root_pixmap(display, root_window, esetroot_pixmap);
	}
}

void XImlib2Background::SetOneImage()
{
	Imlib_Image image = None;
	DesktopConfig * dConfig =
			dynamic_cast<DesktopConfig *>(config);
	
	string file = dConfig->getFileBackground();
	
	if(file != "None"){
		image = imlib_load_image(file.c_str());
	}
	
	   if(image)
	    SetMultiImage(image);
}


void XImlib2Background::SetMultiImage(Imlib_Image image)
{
      Pixmap pmap = None, mask = None;
      GC gc;
      XGCValues gcv;
      XColor xcolor;
      int w, h, x, y, widthOfScreen, heightOfScreen, screen, depth;
      char *bgcolor = NULL;
      Colormap colormap;
      Display* display;
      Window rootWindow;
      string mode;
      
      DesktopConfig * dConfig =
		    dynamic_cast<DesktopConfig *>(config);
       
      XDesktopContainer * xContainer =
                    dynamic_cast<XDesktopContainer *>(container);
      
      display = xContainer->getDisplay();
      rootWindow = xContainer->getRootWindow();	
      screen = DefaultScreen(display);
      depth =  DefaultDepth(display, screen);
      colormap = DefaultColormap(display, screen);
      widthOfScreen = xContainer->widthOfScreen();
      heightOfScreen = xContainer->heightOfScreen();
      mode = dConfig->getModeBackground();
      
      pixmap = XCreatePixmap(display, rootWindow , widthOfScreen, heightOfScreen, depth);
      
      string color = dConfig->getColorBackground();
      
      if(color != "None"){
      	gcv.foreground = gcv.background = BlackPixel(display, screen);
      
      	bgcolor = (char*)dConfig->getColorBackground().c_str();
      
      	if (bgcolor && XParseColor(display, colormap , bgcolor, &xcolor) && XAllocColor(display,  colormap, &xcolor)) {
	      gcv.foreground = gcv.background = xcolor.pixel;
      	  }
        }
      
      gc = XCreateGC(display, pixmap, (GCForeground | GCBackground), &gcv);
      
        if(image){
		
		imlib_context_set_image(image);
		
		if (mode == "SCALE") {
			w = widthOfScreen;
			h = heightOfScreen;
		} else if (mode == "MIRROR") {
			w = imlib_image_get_width() * 2;
			h = imlib_image_get_height() * 2;
		} else if (mode == "FIT") {
			double x_ratio, y_ratio;
			x_ratio = ((double) widthOfScreen) / ((double)  imlib_image_get_width());
			y_ratio = ((double) heightOfScreen) / ((double) imlib_image_get_height());
			if (x_ratio > y_ratio) {
				x_ratio = y_ratio;
			}
			w = (int) (imlib_image_get_width() * x_ratio);
			h = (int) (imlib_image_get_height() * x_ratio);
		} else {
			w = imlib_image_get_width();
			h = imlib_image_get_height();
		}
      	
		if (mode == "SCALE") {
			XFillRectangle(display, pixmap, gc, 0, 0, w, h);
		}
		if (mode == "CENTER" || mode == "FIT") {
			XFillRectangle(display, pixmap, gc, 0, 0, widthOfScreen, heightOfScreen);
			x = (widthOfScreen - w) >> 1;
			y = (heightOfScreen - h) >> 1;
		} else {
			x = 0;
			y = 0;
		}
		
		imlib_context_set_anti_alias(1);
		imlib_context_set_dither(1);
		imlib_context_set_blend(0);
		
		if (mode == "MIRROR") {
			pmap = XCreatePixmap(display, rootWindow, w, h, depth);
			imlib_context_set_drawable(pmap);
			imlib_render_image_on_drawable(0, 0);
			imlib_image_flip_horizontal();
			imlib_render_image_on_drawable(imlib_image_get_width(), 0);
			imlib_image_flip_vertical();
			imlib_render_image_on_drawable(imlib_image_get_width(), imlib_image_get_height());
			imlib_image_flip_horizontal();
			imlib_render_image_on_drawable(0, imlib_image_get_height());
			imlib_context_set_drawable(rootWindow);
		} else {
			imlib_context_set_drawable(rootWindow);
			imlib_render_pixmaps_for_whole_image_at_size(&pmap, &mask, w, h);
		}
	} else {
		x = 0;
		y= 0;
	}
	
	  if (pmap != None) {
		  if (mask) {
			  XFreePixmap(display, mask);
			  mask = None;
		  }  
	   	XSetTile(display, gc, pmap);	
	  }  
	  
	  XSetTSOrigin(display, gc, x, y);
	  XSetFillStyle(display, gc, FillTiled);
	  
	  if (mode == "CENTER" || mode == "FIT") {
		  XFillRectangle(display, pixmap, gc, x, y, w, h);
	  } else {
		  XFillRectangle(display, pixmap, gc, 0, 0, widthOfScreen, heightOfScreen);
	  }
	  
	  esetroot_pixmap_property(rootWindow, pixmap, gc , widthOfScreen, heightOfScreen);
	 
	 if (image)
		  imlib_free_image(); 
	
	  if(pmap)
		 XFreePixmap(display, pmap);
	  if(pixmap)
		imlib_free_pixmap_and_mask(pixmap);
	  pixmap = None;
	  
	  if(gc)
		XFreeGC(display, gc);
}

void XImlib2Background::Refresh(Pixmap currentPixmap){
	
	XDesktopContainer * xContainer =
			dynamic_cast<XDesktopContainer *>(container);
	
	if(spareRoot) {
		imlib_context_set_image(spareRoot);
		imlib_free_image();
	}
	
	if(currentPixmap){
		imlib_context_set_drawable(currentPixmap);
		 spareRoot = imlib_create_image_from_drawable( 0 , 0, 0, 
				xContainer->widthOfScreen(), xContainer->heightOfScreen(), 1);
	}
	  
}

Imlib_Image XImlib2Background::createCropImage(int fX, int fY, int src_width, int src_height, int dest_width, int dest_height){
	if(!spareRoot){
		Pixmap pmap = GetRootPixmap(None);
		Refresh(pmap);
	}
	imlib_context_set_image(spareRoot);
	return imlib_create_cropped_scaled_image(fX, fY, src_width, src_height, dest_width, dest_height);
}

Pixmap XImlib2Background::GetRootPixmap(Atom id){
     Pixmap currentRootPixmap = None;
     XDesktopContainer * xContainer =
			dynamic_cast<XDesktopContainer *>(container);
			
    if( id == None ) id = XInternAtom (xContainer->getDisplay(), "_XROOTPMAP_ID", True);

    if( id != None )
    {
      Atom act_type;
      int act_format;
      unsigned long nitems, bytes_after;
      unsigned char *prop = NULL;

      if (XGetWindowProperty (xContainer->getDisplay(), 
				xContainer->getRootWindow(), id, 0, 1, False, XA_PIXMAP,
			        &act_type, &act_format, &nitems, &bytes_after,
				&prop) == Success)
	{
	    if (prop)
	    {
		currentRootPixmap = *((Pixmap *) prop);
		XFree (prop);
	    }
	}
    }
    return currentRootPixmap ;
}

void XImlib2Background::InitSpareRoot(Window win_context){
	XDesktopContainer * xContainer =
			dynamic_cast<XDesktopContainer *>(container);
	imlib_context_set_drawable(win_context);
	Window tmp = createWindow();
	XMapWindow(xContainer->getDisplay(), tmp);
	spareRoot = imlib_create_image_from_drawable(0 , 0, 0, 
			xContainer->widthOfScreen(), xContainer->heightOfScreen(), 1);     
	XDestroyWindow(xContainer->getDisplay(), tmp);
	imlib_context_set_drawable(xContainer->getRootWindow());
}

Window XImlib2Background::createWindow()
{
	XDesktopContainer * xContainer =
			dynamic_cast<XDesktopContainer *>(container);
	XSetWindowAttributes attr;
	attr.background_pixmap = ParentRelative;
	attr.backing_store = Always;
	attr.event_mask = ExposureMask;
	attr.override_redirect = True;
	Window r = XCreateWindow( xContainer->getDisplay(), xContainer->getRootWindow(), 0, 0, xContainer->widthOfScreen(), xContainer->heightOfScreen(), 0,
				  CopyFromParent, CopyFromParent, CopyFromParent,
				  CWBackPixmap|CWBackingStore|CWOverrideRedirect|CWEventMask,&attr );
	return r;		
}


void XImlib2Background::SetDelay( int d )
{
	if( d>=0 && d<= 1440 ) {
		delay = d;
		if(show->size() + save->size() <= 1) return;
		if( !delay )
			Timer::Remove( this );
		else 
			Timer::Add( this, delay * 60);
	}
}

void XImlib2Background::FreeData()
{
	for( int n=0; n<2; n++ ) {
		for( int i=0; i<data[n].size(); i++ )
			free( data[n][i] );
		data[n].clear();
	}
}

void XImlib2Background::OnTime()
{      
		  ChangeImage();
}

void XImlib2Background::ChangeImage()
{
	Imlib_Image image = NULL;
	int c;

	if( !show->size() && save->size() )
		SwapData();
	
	if( show->size() > 0 ) {
		string sf;
		while( true ) {
			char *f;
			int c = (int)((float)(show->size()-1) * rand() / (RAND_MAX+1.0));
			image =  imlib_load_image(f = (*show)[c] );
	
			//cerr << "[" << c << "/" << show->size() << "]" << f << endl;
			show->erase( show->begin() + c );
			if( image ) {
				save->push_back(f);
				break;
			}

			sf = string("|") + f + "|";
			c = images.find( sf.c_str() );
			if( c >= 0 ) images.erase( c, sf.length() );
			free( f );

			if( !show->size() )
				SwapData();
			
			//cerr << "[" << c << "/" << save->size() << "]" << f << endl;
		}
	}
	

	if (image)
	{
		SetMultiImage(image);
	}
	ScanImages();
}

void XImlib2Background::SwapData()
{
	vector<char*> *p = show;
	show = save;
	save = p;
}

void XImlib2Background::ScanImages()
{
	
	struct stat b;
	
	if( stat( directory.c_str(), &b ) < 0 ) {
		cerr << "[idesk] Background's source not found." << endl;
	}
	
	
	if( srctime == b.st_mtime ) return;
	srctime = b.st_mtime;
	
	if( !S_ISDIR( b.st_mode ) )
		show->push_back( strdup( directory.c_str() ) );
	else {
		struct dirent **files;
		int p, n = scandir( directory.c_str(), &files, 0, 0 );
	        directory += "/";
		
		

		for( int i=0; i<n; free( files[i] ), i++ ) {
			
			string filename = directory + files[i]->d_name;
			if( stat( filename.c_str(), &b ) < 0 ) continue;
			if( S_ISDIR( b.st_mode ) ) {
				continue; 
			}
		
			//for doesn't repeat the same image
			p = images.find( "|" + filename + "|" );
			if( p >= 0 ) continue;
			images += "|" + filename + "|";
			show->push_back(strdup( filename.c_str() ));
		}
	}
}
