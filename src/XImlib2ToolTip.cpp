/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2ToolTip.cpp
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

#include "XImlib2ToolTip.h"
#include "Database.h"

XImlib2ToolTip::XImlib2ToolTip(AbstractContainer * c, AbstractIcon * iPa, AbstractConfig * con, AbstractIconConfig * iCon): 
  container(c), iParent(iPa), config(con), iConfig(iCon)
{

   XDesktopContainer * xContainer =
                    dynamic_cast<XDesktopContainer *>(container);

   DesktopIconConfig * dIconConfig =
		    dynamic_cast<DesktopIconConfig *>(iConfig);
		    
   DesktopConfig * dConfig =
		    dynamic_cast<DesktopConfig *>(config);

   display = xContainer->getDisplay();
   screen = DefaultScreen(display);
   root = xContainer->getRootWindow();
   visual = imlib_context_get_visual();
   cmap = imlib_context_get_colormap();

   captionTipPlacement = dConfig->getCaptionTipPlacement();
    //captionTipOnHover
   captionTipOnHover = dConfig->getCaptionTipOnHover();
	
    //grab the tip Text
   tipText =  dIconConfig->getCaptionTip();
   if(tipText == "" )
   tipText = dIconConfig->getCaption();
}

XImlib2ToolTip::~XImlib2ToolTip()
{
}

void XImlib2ToolTip::createWindow()
{

    XGCValues gcv; 
    XSetWindowAttributes attr_tol;  
    attr_tol.override_redirect = True;
    attr_tol.event_mask =  NoEventMask;
    attr_tol.background_pixel = backColor.pixel;

    tooltip.window = XCreateWindow(display, 
				   root, 
				   1, 
				   1, 
				   10, 
				   10, 
				   1, 
				   DefaultDepth (display, screen),
				   CopyFromParent, 
				   visual,
				   CWOverrideRedirect|CWBackPixel, 
				   &attr_tol);

    pixmap = XCreatePixmap(display, tooltip.window,
                           width, height,
			   DefaultDepth(display,screen));

   fontDrawHandle = XftDrawCreate(display , tooltip.window, visual, cmap);
    
   gcv.function = GXcopy;
    
   tooltip.gc = XCreateGC (display, root , GCFunction| GCBackground, &gcv);   
}

void XImlib2ToolTip::createFont()
{
  
    DesktopIconConfig * dIconConfig =
		    dynamic_cast<DesktopIconConfig *>(iConfig);
		    
    DesktopConfig * dConfig =
		    dynamic_cast<DesktopConfig *>(config);

   tooltip.font = XftFontOpen(display,
		 screen, 
		 XFT_FAMILY,
		 XftTypeString,
		 dConfig->getFontTip().c_str(),
		 XFT_SIZE,
		 XftTypeDouble,
		 (double)dConfig->getFontSizeTip(), 
		 XFT_WEIGHT, XftTypeInteger, XFT_WEIGHT_MEDIUM,
		 NULL );

    XftTextExtentsUtf8(display, tooltip.font, (XftChar8*)tipText.c_str(), tipText.length(), &fontInfo );
    
    width = fontInfo.width + tipText.length() + 4;
    height = tooltip.font->height;
   
    XftColorAllocName(display, visual, cmap, (char*)dConfig->getBackColorTip().c_str(), &backColor );
    
    XftColorAllocName(display, visual, cmap, (char*)dConfig->getForeColorTip().c_str(), &foreColor );

}

Window XImlib2ToolTip::getWindow()
{
  return tooltip.window;
}

void XImlib2ToolTip::event_enter_notify ()
{
	if(captionTipOnHover && tipText.length() > 0){ 
		int fX, fY;		 
		setXY(fX, fY, width, height);
		
		XMoveResizeWindow (display , tooltip.window, fX, fY, width, height);
		XMapRaised (display, tooltip.window);
		
		XftDrawStringUtf8( fontDrawHandle,  &foreColor, tooltip.font, 2, tooltip.font->ascent, (XftChar8*)tipText.c_str(), tipText.length() );
		XSetFillStyle(display, tooltip.gc, FillTiled);
		XSetTile(display, tooltip.gc, pixmap);
		XFillRectangle(display, pixmap, tooltip.gc, 0, 0, width, height);
	}	
}

void XImlib2ToolTip::event_leave_notify ()
{
	if(captionTipOnHover && tipText.length() > 0){ 
		XClearWindow (display, tooltip.window);
		XUnmapWindow (display, tooltip.window);
	}
}


void XImlib2ToolTip::setXY(int & fX, int & fY, int w, int h)
{    

	XDesktopContainer * xContainer =
	       	    dynamic_cast<XDesktopContainer *>(container);

        DesktopIconConfig * dIconConfig =
		    dynamic_cast<DesktopIconConfig *>(iConfig);
	
       int width = iParent->getWidth();
       int height = iParent->getHeight();

    //TODO fix pixel placement

       int ax = xContainer->widthOfScreen()/2;
       int ay = xContainer->heightOfScreen()/2;

	if (captionTipPlacement == "Right")
		fX = iParent->getX() + width + 10;
	else if (captionTipPlacement == "Left")
		fX = iParent->getX() - w - 4;
	else if (captionTipPlacement == "Auto")
	{
	     if (iParent->getX() < ax)
		  fX = iParent->getX() + width + 10;
	     if (iParent->getX() > ax)
		  fX = iParent->getX() - w - 4;
	}
	else //(captionPlacement == "Bottom" || captionPlacement == "Top")
		fX = iParent->getX() + (width / 2) - (w / 2) - 1;

	if (captionTipPlacement == "Left" || captionTipPlacement == "Right")
		fY = iParent->getY() + (height - h)/2;
	else if (captionTipPlacement == "Top")
	        fY = iParent->getY() - (h + 2);
	else //(captionPlacement == "Bottom")
		fY = iParent->getY()+ (height + h + 2);
}
