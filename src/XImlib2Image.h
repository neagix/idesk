/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2Image.h
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

#ifndef XIMLIB2_IMAGE_CLASS
#define XIMLIB2_IMAGE_CLASS

#include "AbstractClasses.h"
#include "DesktopConfig.h"
#include "DesktopIconConfig.h"
#include "XImlib2ToolTip.h"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <Imlib2.h>

class XImlib2Image : public AbstractImage
{
    protected:
    Window window;
    Window rootWindow;
    Visual * visual;
    int depth;
    int screen;
    int widthOfScreen;
    int heightOfScreen;
    Display* display;
    Colormap cmap;
    XImlib2ToolTip * tooltip;
     
    int x, y, orgWidth, orgHeight;

    bool hasAlpha, visi;
    bool valid;
    
	/* Test para SVG*/
    unsigned char *rgb, *alpha, *alpha2;
	
    bool glowing, glowChange;

    int transparency;
    Imlib_Color_Modifier colorMod;

    Imlib_Image image;
       
    string fillStyle;

    static unsigned char mapZeroAlpha[256];
    static unsigned char mapNone[256];

    unsigned char defaultTransTable[256];

    public:
        XImlib2Image(AbstractContainer * c, AbstractIcon * iParent,
                    AbstractConfig * con, AbstractIconConfig * iConfig);
        ~XImlib2Image();

        virtual void draw();

        virtual void configure();
        virtual void initalize();
        virtual void createPicture();

        virtual Window * getWindow();
    	virtual Window getWindowToolTip();
        virtual void createWindow();
	virtual void setupLayer();
        virtual void lowerWindow();
        virtual void mapWindow();
        virtual void unmapWindow();
        virtual void moveWindow(int xCord, int yCord);

        virtual void refreshIcon();
        virtual void repaint();
        virtual void applyMouseOverEffects();

	virtual void mouseOverEffect();
	virtual void mouseOffEffect();
        
        virtual void createTransTables();
        virtual void shapeWindow();
        virtual void createToolTip();
	virtual void redraw();
        virtual void pressImage();
	virtual void unpressImage();
	void event_enter_notify ();
	void event_leave_notify ();
};

#endif
