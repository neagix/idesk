/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XIcon.h
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

#ifndef XICON_CLASS
#define XICON_CLASS

#include "AbstractClasses.h"
#include "XDesktopContainer.h"
#include "XImlib2Image.h"
#include "XImlib2Caption.h"
#include "DesktopConfig.h"
#include "DesktopIconConfig.h"
#include "XImlib2Background.h"
#include <Imlib2.h>

class XIcon : public AbstractIcon
{
    protected:
        virtual void findSnapPosition(int &xCord, int &yCord);
        int lX, lY;
	
    public:
        XIcon(AbstractContainer * cont, AbstractConfig * config,
              AbstractIconConfig * iconConfig);

        ~XIcon();

        bool setImage(AbstractImage *);
        bool setCaption(AbstractCaption *);

	void mouseOverEffect();
	void mouseOffEffect();

        virtual void draw();
	virtual void redraw();
	
        void updateText();
        void save();

        void show();

        Window * getImageWindow();
        Window * getCaptionWindow();
	Window   getToolTipWindow();
	
        void moveImageWindow();
        void mapImageWindow();
        void mapCaptionWindow();
        void initMapCaptionWindow();
        
        AbstractImage * getImage();
        AbstractCaption * getCaption();

        int getX() { return x; }
        int getY() { return y; }
        int getHeight() { return image->getHeight(); }
        int getWidth() { return image->getWidth(); }
        int getFontHeight();

        void fixPosition(int &, int &);
        int captionXShift();
        int captionYShift();
  
        void setX(int xCord) { x = xCord; }
        void setY(int yCord) { y = yCord; }

        bool isDragging() { return dragging; }
        bool isDragged() { return dragged; }

        virtual void dragButtonPress(XEvent ev);
        virtual void dragButtonRelease(XEvent ev);
        virtual void dragMotionNotify(XEvent ev);

        virtual void lowerIcon();
	
	bool createIcon();
	void event_enter_notify ();
	void event_leave_notify ();
	void pressImage();
	void unpressImage();
};

#endif
