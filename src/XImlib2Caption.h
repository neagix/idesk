/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2Caption.h
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

#ifndef XCAPTION_CLASS
#define XCAPTION_CLASS

#include <X11/Xft/Xft.h>

#include <X11/Xlib.h>
#include <Imlib2.h>

#include "AbstractClasses.h"
#include "XDesktopContainer.h"
#include "XImlib2Image.h"



class XImlib2Caption : public AbstractCaption, XImlib2Image
{
    protected:
        XGlyphInfo fontInfo;
        XftDraw * fontDrawHandle;
        
        XftFont * font;
        XftColor fontColor;

        Visual * visual;
        Colormap cmap;
	
	GC gc;
	Pixmap pixmap;

        const int FONTSHIFT;

        int boldVal;

        string shadowColor;
        bool shadowOn;
        int shadowX, shadowY;
        XftFont * shadowFont;
        XftColor shadowFontColor;

        bool captionOnHover;
        string captionPlacement;
        
    public:
        XImlib2Caption();
        XImlib2Caption(AbstractContainer * c, AbstractIcon * iParent,
                 AbstractConfig *, AbstractIconConfig *);
        ~XImlib2Caption();

        void draw();
        void updateText();

        virtual Window * getWindow();
        virtual void createWindow();
        virtual void lowerWindow();
        virtual void mapWindow();
        virtual void initMapWindow();
        virtual void unmapWindow();
        virtual void moveWindow(int xC, int yC);

        int getFontWidth();
        int getFontHeight();
        
        void configure();
        void createFont();
        
        virtual void mouseOverEffect();
        virtual void mouseOffEffect();

        virtual void setXY(int &fX, int &fY);
        virtual void renderFont2Imlib();
};

#endif
