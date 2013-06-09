/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2Image.cpp
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
#include "XImlib2Image.h"
#include "XIcon.h"

XImlib2Image::XImlib2Image(AbstractContainer * c, AbstractIcon * iParent,
                         AbstractConfig * con, AbstractIconConfig * iConfig)
                            : AbstractImage(c, iParent, con, iConfig),
                              hasAlpha(false), glowing(false)
{   
}

XImlib2Image::~XImlib2Image()
{   
}

void XImlib2Image::configure()
{
    x = iconParent->getX();
    y = iconParent->getY();

    DesktopIconConfig * dIconConfig =
        dynamic_cast<DesktopIconConfig *>(iconConfig);
	
    DesktopConfig * dConfig =
		    dynamic_cast<DesktopConfig *>(config);

    transparency = dConfig->getTransparency();

    width = dIconConfig->getWidth();
    height = dIconConfig->getHeight();
    
    glowing = false;
    glowChange = true;

    colorMod = imlib_create_color_modifier();
    
    fillStyle = dIconConfig->getFillStyle();
    
    XDesktopContainer * xContainer =
		    dynamic_cast<XDesktopContainer *>(container);  
	
    rootWindow = xContainer->getRootWindow();
    widthOfScreen = xContainer->widthOfScreen();
    heightOfScreen = xContainer->heightOfScreen();
    display = xContainer->getDisplay();
    visual = imlib_context_get_visual();
    cmap = imlib_context_get_colormap();
    screen = DefaultScreen(display);
    depth =  DefaultDepth(display,screen);
}

void XImlib2Image::createPicture()
{
	DesktopIconConfig * dIconConfig =
			dynamic_cast<DesktopIconConfig *>(iconConfig);
	
	image = imlib_load_image(dIconConfig->getPictureFilename().c_str());

	if (image)
	{
		imlib_context_set_image(image);

		orgWidth = imlib_image_get_width();
		orgHeight = imlib_image_get_height();

		if (width == 0)
			width = imlib_image_get_width();
		if (height == 0)
			height= imlib_image_get_height();
		
		Imlib_Image tempImg = imlib_create_cropped_scaled_image(0, 0,
				orgWidth, orgHeight, width, height);

		imlib_free_image();

		image = tempImg;
        
		hasAlpha = true; //TODO add checks here
	}    
	else
	{
		cout << "Can't load: " << dIconConfig->getPictureFilename()
				<< " bailing -- "
				<< dIconConfig->getCaption() << endl
				<< "Check to see if the image and path to image is valid\n";
	}
	
}

Window * XImlib2Image::getWindow()
{
    return &window;
}

void XImlib2Image::createWindow()
{	
    createPicture();

    DesktopConfig * dConfig =
		    dynamic_cast<DesktopConfig *>(config);
    

    XSetWindowAttributes attr;
    attr.background_pixmap = ParentRelative;
    attr.backing_store = Always;
    attr.override_redirect = True;
    attr.save_under = True;
    attr.cursor = XCreateFontCursor(display, dConfig->getCursorOver());
  
    attr.event_mask = SubstructureRedirectMask |
                      SubstructureNotifyMask   |
                      ButtonPressMask          |
                      ButtonReleaseMask        |
                      PointerMotionMask        |
                      EnterWindowMask          |
                      PropertyChangeMask       |
                      LeaveWindowMask           |
                      ExposureMask;
    
    window = XCreateWindow( display,
                            rootWindow,
                            0,
                            0,
                            width,
                            height,
                            0,
                            depth,
                            CopyFromParent,
                            CopyFromParent,
			    CWSaveUnder|CWBackPixmap|CWBackingStore|CWOverrideRedirect|CWEventMask| CWCursor,
                            &attr );  
    shapeWindow();
}

void XImlib2Image::createToolTip()
{
  tooltip = new XImlib2ToolTip(container,iconParent,config,iconConfig);
  tooltip->createFont();
  tooltip->createWindow();
}

Window XImlib2Image::getWindowToolTip()
{
  return tooltip->getWindow();
}

void XImlib2Image::setupLayer() {
  
    XDesktopContainer * xContainer =
            dynamic_cast<XDesktopContainer *>(container);
	    
    Display *disp = xContainer->getDisplay();

    Atom type = XInternAtom(disp, "_NET_WM_WINDOW_TYPE", false);
    Atom state = XInternAtom(disp, "_NET_WM_STATE", false);
    
    Atom state_atoms[] = {
        XInternAtom(disp, "_NET_WM_STATE_BELOW", false),
        XInternAtom(disp, "_NET_WM_STATE_HIDDEN", false),
        XInternAtom(disp, "_NET_WM_STATE_SKIP_PAGER", false),
        XInternAtom(disp, "_NET_WM_STATE_SKIP_TASKBAR", false),
        XInternAtom(disp, "_NET_WM_STATE_STICKY", false)
    };

    Atom type_atoms =  XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DESKTOP", false);

    XChangeProperty(disp, window, state, XA_ATOM,
                                 32, PropModeReplace,
                                 (unsigned char *)(state_atoms), 5);
    XChangeProperty(disp, window, type, XA_ATOM,
                                 32, PropModeReplace,
                                 (unsigned char *)(&type_atoms), 1);

    XChangeProperty(disp, window, state, XA_ATOM,
                                  32, PropModeReplace,
                                  (unsigned char *)(state_atoms), 5);
    
    XChangeProperty(disp, window, type, XA_ATOM,
                                  32, PropModeReplace,
                                  (unsigned char *)(&type_atoms), 1);

}


void XImlib2Image::shapeWindow()
{
	
     XDesktopContainer * xContainer = dynamic_cast<XDesktopContainer *>(container);
     
    //Shape the window to the icon
    imlib_context_set_image(image);

    imlib_context_set_color_modifier(colorMod);
    imlib_set_color_modifier_tables(mapNone, mapNone, mapNone, mapZeroAlpha);
    Pixmap pixmap, pixMask;
    imlib_render_pixmaps_for_whole_image(&pixmap, &pixMask);
    XSetWindowBackgroundPixmap (display, window, pixmap);
    
#ifdef SHAPE
    XShapeCombineMask(display, window, ShapeBounding, 0, 0, pixMask, ShapeSet);
#endif // SHAPE
    
    imlib_reset_color_modifier();
    imlib_free_pixmap_and_mask(pixmap);
}

void XImlib2Image::lowerWindow()
{
    XLowerWindow( display , window  );
}

void XImlib2Image::moveWindow(int xCord, int yCord)
{
    XIcon * xIcon = dynamic_cast<XIcon *>(iconParent);

    if (xCord >= 0 &&
        yCord >= 0 &&
        xCord < widthOfScreen &&
        yCord < heightOfScreen )
    {
        x = xCord;
        y = yCord;
        xIcon->setX(x);
        xIcon->setY(y);
         
	 XMoveWindow( display , window, x, y );
    }
    else
    {
        x = widthOfScreen/2;
        y = heightOfScreen/2;
        xIcon->setX(x);
        xIcon->setY(y);
	XMoveWindow( display , window, x, y );
    }
}

void XImlib2Image::redraw()
{
}

void XImlib2Image::draw()
{
    refreshIcon();
}

void XImlib2Image::unmapWindow()
{
    XUnmapWindow( display, window );
}

void XImlib2Image::mapWindow()
{
    XMapWindow( display , window );
}

void XImlib2Image::refreshIcon()
{
    imlib_context_set_image(image);
    x = iconParent->getX();
    y = iconParent->getY();
	
    applyMouseOverEffects();
    repaint();
}

void XImlib2Image::repaint()
{
	
    XDesktopContainer * xContainer =
	dynamic_cast<XDesktopContainer *>(container);
    
    Imlib_Image cropImage = xContainer->bg->createCropImage(x, y, width, height, width, height);
    
    imlib_context_set_dither(1);

    imlib_context_set_blend(1);       //automatically blend image and background
    imlib_context_set_dither_mask(0);
    imlib_context_set_image(cropImage);
    imlib_blend_image_onto_image(image, 1, 0, 0, width, height, 0, 0, width, height);
    
    imlib_image_set_has_alpha(1);
    
    imlib_context_set_anti_alias(1);  //smoother scaling
    imlib_context_set_blend(0);    
    
    imlib_context_set_drawable(window);
    imlib_render_image_on_drawable_at_size(0, 0, width, height);
    
    imlib_free_image();
    imlib_context_set_drawable(xContainer->getRootWindow());
}

void XImlib2Image::applyMouseOverEffects()
{
    if (glowChange)
    {
	        imlib_context_set_color_modifier(colorMod);
		imlib_get_color_modifier_tables(mapNone, mapNone, mapNone, NULL);
		imlib_reset_color_modifier();

	if (glowing == true)  // reset to standard alpha mapping
           imlib_set_color_modifier_tables(mapNone, mapNone, mapNone, mapNone);
	else  // remap alpha values lower to increase transparency
	       imlib_set_color_modifier_tables(mapNone, mapNone, mapNone, defaultTransTable);
	
        glowChange = false;
    }
}

void XImlib2Image::mouseOverEffect()
{
    glowing = true;
    glowChange = true;
    refreshIcon();
}

void XImlib2Image::mouseOffEffect()
{
    glowing = false;
    glowChange = true;
    refreshIcon();
}


void XImlib2Image::event_enter_notify ()
{
   if(tooltip)
     tooltip->event_enter_notify();
}

void XImlib2Image::event_leave_notify ()
{
  if(tooltip)
    tooltip->event_leave_notify(); 
}

void XImlib2Image::unpressImage(){
     if(fillStyle != "None"){
         mouseOffEffect();
         event_leave_notify(); 
      }  
}

void XImlib2Image::pressImage(){
	    XGCValues values;
	    values.function = GXinvert;
	    values.fill_style =  FillSolid;
	    values.background =  WhitePixel(display, depth);
	    values.foreground =  BlackPixel(display, depth);

            Pixmap pixmap = XCreatePixmap(display, window, width, height, depth);
	    GC gcPress = XCreateGC(display, pixmap, GCFillStyle | GCFunction| GCForeground | GCBackground, &values);
	 
	    if(fillStyle == "FillInvert"){
            mouseOverEffect();
	        XFillRectangle(display, window, gcPress, 0, 0, width, height);
	    }
	    else if(fillStyle == "FillHLine"){  
            mouseOverEffect();
            for (unsigned int i = 0; i < height; i += 3)
                XDrawLine(display, window, gcPress, 0, i, width, i);
	    }	    
	    else if(fillStyle == "FillVLine"){
            mouseOverEffect();
	        for (unsigned int i = 0; i < height; i += 3)
		        XDrawLine(display, window, gcPress, i, 0, i, height);	    
	    }  
        XFreePixmap(display, pixmap);
	XFreeGC(display, gcPress);
}


void XImlib2Image::initalize()
{
    createTransTables();
}

void XImlib2Image::createTransTables()
{
    for (int i = 0; i < 256; i++)
    {
        if (i - transparency > 0)
            defaultTransTable[i] = i - transparency;
        else
            defaultTransTable[i] = 0;
    }
}

// array used to map the first few values of alpha to totally transparent and
// all other pixels to opaque. Used to create the window shape mask
unsigned char XImlib2Image::mapZeroAlpha[256] =
{
    0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255
}
;

// a generic mapping of 0 to 0, 1 to 1, etc
// used to preserve colors while remapping alpha channel
unsigned char XImlib2Image::mapNone[256] =
{
    0  , 1  , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  , 10 , 11 , 12 ,
    13 , 14 , 15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 ,
    26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37 , 38 ,
    39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , 47 , 48 , 49 , 50 , 51 ,
    52 , 53 , 54 , 55 , 56 , 57 , 58 , 59 , 60 , 61 , 62 , 63 , 64 ,
    65 , 66 , 67 , 68 , 69 , 70 , 71 , 72 , 73 , 74 , 75 , 76 , 77 ,
    78 , 79 , 80 , 81 , 82 , 83 , 84 , 85 , 86 , 87 , 88 , 89 , 90 ,
    91 , 92 , 93 , 94 , 95 , 96 , 97 , 98 , 99 , 100, 101, 102, 103,
    104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
    117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
    143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
    156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
    169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
    182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
    195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
    221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
    234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
    247, 248, 249, 250, 251, 252, 253, 254, 255 
};
