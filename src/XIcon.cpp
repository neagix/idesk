/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XIcon.cpp
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

#include <fstream>
#include "XIcon.h"
#include "XImlib2Shadow.h"
#include "XImlib2Image.h"

XIcon::XIcon(AbstractContainer * cont, AbstractConfig * con,
              AbstractIconConfig * iConfig) : AbstractIcon(cont, con, iConfig)
{
    DesktopIconConfig * dIcon = dynamic_cast<DesktopIconConfig *>(iconConfig);
   
    container = cont;
    valid = true;
    iconFilename = dIcon->getIconFilename();
    commandArray = dIcon->getCommandArray();
    x = dIcon->getX();
    y = dIcon->getY();
    lX = lY = 0;

    dragged = dragging = false;

    if (dIcon->isRaster())
        image = new XImlib2Image(cont, this, config, iconConfig);
    else
    {
	cout << "Unknown file format: " << iconConfig->getPictureFilename() << "\n" << endl;
	valid = false;
        // implement way to skip icon and not segfault
    }
}

XIcon::~XIcon()
{
    delete image;
    if (captionOn)
        delete caption;
}

bool XIcon::createIcon()
{
	ifstream filein( iconConfig->getPictureFilename().c_str());
	if (filein.fail()){
		cout << "Can't load: " << iconConfig->getPictureFilename()
				<< " bailing -- "
				<< iconConfig->getCaption() << endl
				<< "Check to see if the image and path to image is valid\n"; 
		return false;
	}	
	
	XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image);
	
	xImage->configure();
	xImage->createWindow();
        xImage->createToolTip();
	xImage->setupLayer();
	xImage->initalize();
	xImage->lowerWindow();	
    
	if (iconConfig->getCaption() == "" || iconConfig->getCaption() == " ")
		captionOn = false;
	else
	{
		captionOn = true;
		caption = new XImlib2Caption(container, this, config, iconConfig);
		XImlib2Caption * xCaption = dynamic_cast<XImlib2Caption *>(caption);
		xCaption->createWindow();
                xCaption->renderFont2Imlib();
		xCaption->lowerWindow();
	}
	return true;
}

void XIcon::lowerIcon()
{
    XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image);
   
    if(xImage) 
      xImage->lowerWindow();

    if (captionOn)
    {
        XImlib2Caption * xCaption = dynamic_cast<XImlib2Caption *>(caption);
        if(xCaption) 
          xCaption->lowerWindow();
    }
}

Window XIcon::getToolTipWindow()
{
	return dynamic_cast<XImlib2Image *>(image)->getWindowToolTip();
}

Window * XIcon::getImageWindow()
{
    return dynamic_cast<XImlib2Image *>(image)->getWindow();
}

Window * XIcon::getCaptionWindow()
{
    if (captionOn)
        return dynamic_cast<XImlib2Caption *>(caption)->getWindow();
    else
        return NULL;
}

AbstractImage * XIcon::getImage()
{
    return image;
}

AbstractCaption * XIcon::getCaption()
{
    if (captionOn)
        return caption;
    else
        return NULL;
}

void XIcon::dragButtonPress(XEvent ev)
{
    lowerIcon();
    lX = ev.xbutton.x_root;
    lY = ev.xbutton.y_root;
    dragging = True;
}

void XIcon::dragMotionNotify(XEvent ev)
{

    dragged = True;

    x  -= lX - ev.xmotion.x_root;
    y  -= lY - ev.xmotion.y_root;
    lX  = ev.xmotion.x_root;
    lY  = ev.xmotion.y_root;
    
    fixPosition(x, y);

    XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image);
    
    if(xImage){ 
        xImage->event_leave_notify();
        xImage->moveWindow(x, y);
        xImage->refreshIcon();
    }

    if (captionOn)
    {
        XImlib2Caption * xCaption = dynamic_cast<XImlib2Caption *>(caption);
        int fX, fY;
        if(xCaption){
            xCaption->setXY(fX, fY);
            xCaption->moveWindow(fX/*captionXShift()*/, fY/*captionYShift()*/);
            xCaption->draw();
        } 
    }
}

void XIcon::dragButtonRelease(XEvent ev)
{	
    if( dragged )
    {
        XDesktopContainer * xContainer =
            dynamic_cast<XDesktopContainer *>(container);
        XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image);
        DesktopConfig * dConfig = dynamic_cast<DesktopConfig *>(config);

        if (dConfig->getSnapState())
        {
            findSnapPosition(x, y);

            fixPosition(x, y);

	    if(xImage){ 
              xImage->moveWindow(x, y);
	    }

            if (captionOn)
            {
                XImlib2Caption * xCaption = dynamic_cast<XImlib2Caption *>(caption);
		if(xCaption){
                  xCaption->moveWindow(captionXShift(), captionYShift());
		}
            }
        }
        draw();
        xContainer->saveIcon(this);
    }
    dragging=False;
    dragged=False;
}   

void XIcon::findSnapPosition(int &xCord, int &yCord)
{   
    DesktopConfig * dConfig =
            dynamic_cast<DesktopConfig *>(config);
    XDesktopContainer * xContainer =
            dynamic_cast<XDesktopContainer *>(container);

	if (!dConfig->getStartSnapLeft())
		xCord = xContainer->widthOfScreen() - xCord;
	if (!dConfig->getStartSnapTop())
		yCord = xContainer->heightOfScreen() - yCord;

	int xMid = xCord + image->getWidth()/2;
	int yMid = yCord + image->getHeight()/2;

	xCord = xMid - xMid % (dConfig->getSnapWidth());
	yCord = yMid - yMid % (dConfig->getSnapHeight());

	xCord += dConfig->getSnapWidth()/2 - image->getWidth()/2;
	yCord += dConfig->getSnapHeight()/2 - image->getHeight()/2;

	if (!dConfig->getStartSnapLeft())
		xCord = xContainer->widthOfScreen() - xCord;
	if (!dConfig->getStartSnapTop())
		yCord = xContainer->heightOfScreen() - yCord;
    
}


bool XIcon::setImage(AbstractImage * img)
{
    bool returnBool = false;

    if (img)
    {
        image = img;
        returnBool = true;
    }
    
    return returnBool;
}

bool XIcon::setCaption(AbstractCaption * cap)
{
    bool returnBool = false;

    if (cap && captionOn)
    {
        caption = cap;
        returnBool = true;
    }
    
    return returnBool;
}


void XIcon::draw()
{
    XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image);
		
    if(xImage){
      image->draw();
    }

    if (captionOn)
    {
        XImlib2Caption * xCaption = dynamic_cast<XImlib2Caption *>(caption);
        if(xCaption)
         xCaption->draw();
    }
}

void XIcon::show()
{
    moveImageWindow();
    mapImageWindow();
    //don't initially map caption for the hover effect
    initMapCaptionWindow();
}

void XIcon::redraw()
{
    if(image)
      image->redraw();
}

void XIcon::updateText()
{
    if (captionOn && caption)
        caption->draw();
}

void XIcon::pressImage()
{
    XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image); 
    if(xImage)
      xImage->pressImage();
}

void XIcon::unpressImage()
{
	XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image); 
    if(xImage)
	  xImage->unpressImage();
}

void XIcon::mouseOverEffect()
{
    if(image)
      image->mouseOverEffect();

    if (captionOn && caption)
        caption->mouseOverEffect();
}

void XIcon::mouseOffEffect()
{
    if(image)
      image->mouseOffEffect();
    
    if (captionOn && caption)
        caption->mouseOffEffect();
}

void XIcon::event_enter_notify ()
{
    if(image)
	  image->event_enter_notify();	
}

void XIcon::event_leave_notify ()
{
    if(image)
	  image->event_leave_notify();	
}

void XIcon::save()
{
    DesktopIconConfig * dIcon = dynamic_cast<DesktopIconConfig *>(iconConfig);
    if(dIcon) 
      dIcon->saveIcon(x, y);
}

void XIcon::fixPosition(int &xCord, int &yCord)
{
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(container);
    
    if( xCord < 0 ) xCord = 0;
    if( yCord < 0 ) yCord = 0;

    if( xCord/* + image->getWidth()*/ > xContainer->widthOfScreen() )
        xCord = xContainer->widthOfScreen()/* - image->getWidth()*/;

    if( yCord/* + image->getHeight()*/ > xContainer->heightOfScreen() )
        yCord = xContainer->heightOfScreen()/* - image->getHeight()*/;
}

int XIcon::captionXShift()
{
    if (captionOn && image && caption)
        return x + ( image->getWidth() - caption->getFontWidth() )/2;
    else
        return 0;
}

int XIcon::captionYShift()
{
    if (captionOn && image)
        return y + image->getHeight() + 4;
    else
        return 0;
}

int XIcon::getFontHeight()
{
    if (captionOn && caption)
        return caption->getFontHeight();
    else
        return 0;
}

void XIcon::moveImageWindow()
{
    XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image);
    if(xImage)
      xImage->moveWindow(x, y);
}

void XIcon::mapImageWindow()
{
    XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image);
    if(xImage)
      xImage->mapWindow();

}

void XIcon::mapCaptionWindow()
{
    if (captionOn)
    {
        XImlib2Caption * xCaption = dynamic_cast<XImlib2Caption *>(caption);
        if(xCaption)
         xCaption->mapWindow();
    }
}

void XIcon::initMapCaptionWindow()
{
    if (captionOn)
    {
        XImlib2Caption * xCaption = dynamic_cast<XImlib2Caption *>(caption);
        if(xCaption)
         xCaption->initMapWindow();
    }
}
