/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2ToolTip.h
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

#ifndef XIMLIB2TOOLTIP_CLASS
#define XIMLIB2TOOLTIP_CLASS

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <Imlib2.h>

#include "DesktopContainer.h"
#include "DesktopConfig.h"
#include "XDesktopContainer.h"

struct Tooltip
{
	Window window;
	XftFont *font;
	GC gc;
};

class XImlib2ToolTip
{
    protected:
        AbstractConfig * config;
        AbstractContainer * container;
        AbstractIconConfig * iConfig;
        AbstractIcon * iParent;

        int width, height;
        XGlyphInfo fontInfo;
	XftColor foreColor, backColor;
        XftDraw * fontDrawHandle;
        Pixmap pixmap;

        Window root;
        Display * display;
        struct Tooltip tooltip;
        Visual * visual;
        Colormap cmap;
        int screen;

        bool captionTipOnHover;
        string captionTipPlacement;

        string tipText;

    public:
	XImlib2ToolTip(AbstractContainer * c,  AbstractIcon * iPa, AbstractConfig * con, AbstractIconConfig * iCon);
	~XImlib2ToolTip();
        virtual void createWindow();
        virtual void createFont();
        virtual void setXY(int &fX, int &fY, int w, int h);
        void event_enter_notify ();
	void event_leave_notify ();
        Window getWindow();
};

#endif
