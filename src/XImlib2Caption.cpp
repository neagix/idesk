/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2Caption.cpp
 *
 * Copyright (c) 2002, Chris (nikon) (nikon@sc.rr.com)
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
#ifdef SHAPE
#include  <X11/extensions/shape.h>
#endif // SHAPE
#include "XImlib2Caption.h"
#include "XIconWithShadow.h"

XImlib2Caption::XImlib2Caption(AbstractContainer * c, AbstractIcon * iParent,
                   AbstractConfig * con, AbstractIconConfig * iConfig) 
           : AbstractCaption(c, iParent, con, iConfig),
             XImlib2Image(c, iParent, con, iConfig), FONTSHIFT(4)
{
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(AbstractImage::container);

    visual = imlib_context_get_visual();
    cmap = imlib_context_get_colormap();
    configure();
    createFont();
}

XImlib2Caption::~XImlib2Caption()
{
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(AbstractImage::container);

   XftColorFree( xContainer->getDisplay(), visual, cmap, &fontColor );

    if (shadowOn)
        XftColorFree( xContainer->getDisplay(), visual, cmap, &shadowFontColor);

    if(gc)
	XFreeGC(xContainer->getDisplay(), gc);
    
     XDestroyWindow(xContainer->getDisplay(), window);
     XftDrawDestroy(fontDrawHandle);
}

void XImlib2Caption::configure()
{
    DesktopIconConfig * dIconConfig =
        dynamic_cast<DesktopIconConfig *>(AbstractImage::iconConfig);

    //grab the text
    text = AbstractImage::iconConfig->getCaption();
    
    //set boldness
    if (dIconConfig->getBoldness())
        boldVal = XFT_WEIGHT_BOLD;
    else
        boldVal = XFT_WEIGHT_MEDIUM;

    //set shadow options
    shadowOn = dIconConfig->isShadow();
    shadowX = dIconConfig->getShadowX();
    shadowY = dIconConfig->getShadowY();
    shadowColor = dIconConfig->getShadowColor();

    //captionOnHover
    captionOnHover = dIconConfig->getCaptionOnHover();
    
    //captionPlacement
    captionPlacement = dIconConfig->getCaptionPlacement();
}

void XImlib2Caption::createFont()
{
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(AbstractImage::container);
    DesktopIconConfig * dIconConfig =
        dynamic_cast<DesktopIconConfig *>(AbstractImage::iconConfig);

    font = XftFontOpen( xContainer->getDisplay(),
                        DefaultScreen( xContainer->getDisplay()), 
                        XFT_FAMILY,
                        XftTypeString,
                        dIconConfig->getFont().c_str(),
                        XFT_SIZE,
                        XftTypeDouble,
                        (double)dIconConfig->getFontSize(), 
                        XFT_WEIGHT, XftTypeInteger, boldVal,
                        NULL );
    
    XColor  screen, exact;
    if (!XAllocNamedColor (xContainer->getDisplay(), cmap, (char*)dIconConfig->getFontColor().c_str(), &screen, &exact))
        cout << "BAD COLORS!!!\n";
    
    XftColorAllocName(xContainer->getDisplay(), visual, cmap,
                      (char*)dIconConfig->getFontColor().c_str(), &fontColor );

    if (shadowOn)
        XftColorAllocName(xContainer->getDisplay(), visual, cmap,
                          (char*)shadowColor.c_str(), &shadowFontColor );
}


Window * XImlib2Caption::getWindow()
{
    return &window;
}

void XImlib2Caption::createWindow()
{
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(AbstractImage::container);

    DesktopConfig * dConfig =
		    dynamic_cast<DesktopConfig *>(XImlib2Image::config);

    XSetWindowAttributes attr;
    attr.override_redirect = True;
    attr.cursor = XCreateFontCursor(xContainer->getDisplay(), dConfig->getCursorOver());
    
    attr.event_mask = SubstructureRedirectMask |
                      SubstructureNotifyMask   |
                      ButtonPressMask          |
                      ButtonReleaseMask        |
                      PointerMotionMask        |
                      PropertyChangeMask       |
                      ExposureMask;

    XftTextExtentsUtf8( xContainer->getDisplay(), font, (XftChar8*)text.c_str(), text.length(), &fontInfo );

    // fix window extents so shadow text is not cut
    // TODO: fix window extents if shadow is negative:
    // Would have to edit x and y of the window
    width = fontInfo.width + FONTSHIFT;
    height = font->height;

    if (shadowOn)
    {
        width += shadowX > 0 ? shadowX : 0;
        height += shadowY > 0 ? shadowY : 0;
    }
    window = XCreateWindow( xContainer->getDisplay(),
                            xContainer->getRootWindow(),
                            0,
                            0,
                            width,
                            height,
                            0,
                            CopyFromParent,
                            CopyFromParent,
                            CopyFromParent,
			    CWOverrideRedirect|CWEventMask|CWCursor,
                            &attr );
}

void XImlib2Caption::renderFont2Imlib()
{
   
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(AbstractImage::container);

    Display * display = xContainer->getDisplay();
    int screen =  DefaultScreen(display);

    pixmap = XCreatePixmap(display, window, width,
                            height,
                           DefaultDepth(display, screen));
    
    XSetWindowBackgroundPixmap (display, window, pixmap);
    
    gc = XCreateGC(display, window, 0, 0);
    
    if( pixmap == (Pixmap) NULL )
    {
	    printf("\nError creando Pixmap\n");
	    XCloseDisplay(display);
	    exit(1);
    }
    
    fontDrawHandle = XftDrawCreate(display, pixmap, visual, cmap);

    XSetFillStyle(display, gc, FillSolid);
    XSetForeground(display, gc, BlackPixel(display, 0));
    XSetBackground(display, gc, WhitePixel(display, 0));
    XFillRectangle(display, pixmap, gc, 0, 0, width, height);
    Pixmap shapeMask = XCreatePixmap(display, window, width, height,1);
    GC tempGc = XCreateGC(display, shapeMask, 0, 0);
    XCopyPlane(display, pixmap, shapeMask, tempGc, 0, 0, width, height, 0, 0, 1);
#ifdef SHAPE					
    XShapeCombineMask(display, window, 0, 0, 0, shapeMask, ShapeUnion);
#endif // SHAPE
    imlib_context_set_drawable(pixmap);
}

void XImlib2Caption::draw()
{
        XDesktopContainer * xContainer =
			dynamic_cast<XDesktopContainer *>(AbstractImage::container);
	XIconWithShadow * sIcon = dynamic_cast<XIconWithShadow *>(AbstractImage::iconParent);

        Display * display = xContainer->getDisplay();
    
	int fX, fY;
    
	setXY(fX, fY);
	
	Imlib_Image cropImage = xContainer->bg->createCropImage(fX, fY, width, height, width, height);
	
	imlib_context_set_image(cropImage);
	
	imlib_context_set_drawable(window);
	imlib_render_image_on_drawable(0, 0);
	
	Pixmap shapeMask;
	imlib_render_pixmaps_for_whole_image (&pixmap, &shapeMask);
	XSetWindowBackgroundPixmap (display, window, pixmap);
	imlib_free_image();
	
	imlib_context_set_drawable(xContainer->getRootWindow());

       	imlib_context_set_image(xContainer->bg->spareRoot);
    
    // if the icon shadow is enabled and the shadow is visible underneath the
    // text window
	if (sIcon && //TODO Fix this
		   sIcon->getShadowX() > fX - sIcon->getWidth() &&
		   sIcon->getShadowY() > fY - sIcon->getHeight() &&
		   sIcon->getShadowX() < fX + sIcon->getWidth() &&
		   sIcon->getShadowY() < fY + sIcon->getHeight() )
		sIcon->renderShadowToImage(pixmap, fX, fY);

	updateText();

	XMoveWindow(display, window, fX, fY );
    
        //apply the buffer to the window
	XSetFillStyle(display, gc, FillTiled);
	XSetTile(display, gc, pixmap);
	XFillRectangle(display, window, gc, 0, 0, width, height);
	imlib_free_pixmap_and_mask(pixmap);
}

void XImlib2Caption::updateText()
{   
     XDesktopContainer * xContainer =
			dynamic_cast<XDesktopContainer *>(AbstractImage::container);
    
      XftDrawChange(fontDrawHandle, pixmap);
	if (shadowOn)
		XftDrawStringUtf8( fontDrawHandle, &shadowFontColor, font, FONTSHIFT/2 + shadowX, font->ascent + shadowY, (XftChar8*)text.c_str(), text.length() );

	XftDrawStringUtf8(fontDrawHandle, &fontColor, font, FONTSHIFT/2, font->ascent, (XftChar8*)text.c_str(), text.length() );
}

void XImlib2Caption::lowerWindow()
{   
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(AbstractImage::container);

    XLowerWindow( xContainer->getDisplay(), window );
}

void XImlib2Caption::setXY(int & fX, int & fY)
{
    //TODO fix pixel placement
    if (captionPlacement == "Right")
        fX = AbstractImage::iconParent->getX() + AbstractImage::iconParent->getWidth() + 10;
    else if (captionPlacement == "Left")
        fX = AbstractImage::iconParent->getX() - fontInfo.width - 10;
    else //(captionPlacement == "Bottom" || captionPlacement == "Top")
        fX = AbstractImage::iconParent->getX() + 
             (AbstractImage::iconParent->getWidth() - fontInfo.width)/2;

    if (captionPlacement == "Left" || captionPlacement == "Right")
        fY = AbstractImage::iconParent->getY() + (AbstractImage::iconParent->getHeight() - fontInfo.height)/2;
    else if (captionPlacement == "Top")
        fY = AbstractImage::iconParent->getY() - fontInfo.height - 2*FONTSHIFT;
    else //(captionPlacement == "Bottom")
        fY = AbstractImage::iconParent->getY() + AbstractImage::iconParent->getHeight() + FONTSHIFT;
}

int XImlib2Caption::getFontWidth()
{
    return fontInfo.width;
}

int XImlib2Caption::getFontHeight()
{
    return font->height;
}
void XImlib2Caption::mapWindow()
{
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(AbstractImage::container);

    XMapWindow( xContainer->getDisplay(), window );
}

void XImlib2Caption::initMapWindow()
{
    if (!captionOnHover)
        mapWindow();
}

void XImlib2Caption::unmapWindow()
{
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(AbstractImage::container);

    XUnmapWindow( xContainer->getDisplay(), window );
}

void XImlib2Caption::moveWindow(int xC, int yC)
{
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(AbstractImage::container);
    setXY(xC, yC);
    XMoveWindow( xContainer->getDisplay(), window, xC, yC );
}

void XImlib2Caption::mouseOverEffect()
{
    if (captionOnHover)
        mapWindow();
}

void XImlib2Caption::mouseOffEffect()
{
    if (captionOnHover)
        unmapWindow();
}
