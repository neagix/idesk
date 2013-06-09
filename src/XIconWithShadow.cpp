/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XIconWithShadow.cpp
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

#include "XIconWithShadow.h"
#include "XImlib2Shadow.h"

XIconWithShadow::XIconWithShadow(AbstractContainer * cont, AbstractConfig * con,
              AbstractIconConfig * iConfig)
	: XIcon(cont, con, iConfig)
{   
    //cout << "XIconWithShadow()\n"; 
    DesktopIconConfig * dIcon = dynamic_cast<DesktopIconConfig *>(iconConfig);
    
    valid = true;

    if (dIcon->isRaster())
        shadowImage = new XImlib2Shadow(cont, this, config, iconConfig);
    else
    {
	cout << "Unknown file format:"  << iconConfig->getPictureFilename() << "\n" << endl;
        // implement way to skip icon and not segfault
	valid = false;
    }
}

XIconWithShadow::~XIconWithShadow()
{
    delete shadowImage;
}

bool XIconWithShadow::createIcon()
{
	if(!XIcon::createIcon()) return false;
	
	XImlib2Shadow * sImage = dynamic_cast<XImlib2Shadow *>(shadowImage);

	shadowVisible = false;
	sImage->configure();
	sImage->createWindow();
	sImage->initalize();
	sImage->lowerWindow();
	return true;
}

void XIconWithShadow::dragMotionNotify(XEvent ev)
{
    XIcon::dragMotionNotify(ev);
    
    //XImlib2Image * sImage = dynamic_cast<XImlib2Image *>(shadowImage);
    //sImage->repaint();
    shadowImage->repaint();
    snapShadow();
}

void XIconWithShadow::snapShadow()
{
    //cout << "XIconWithShadow::snapShadow()\n"; 
    int tempX, tempY;
    XImlib2Shadow * xImage = dynamic_cast<XImlib2Shadow *>(shadowImage);

    if (shadowVisible)
    {   
        tempX = x;
        tempY = y;
        
        findSnapPosition(tempX, tempY);
        fixPosition(tempX, tempY);

        if (tempX != shadowX || tempY != shadowY)
        {
            shadowX = tempX;
            shadowY = tempY;

            xImage->moveWindow(shadowX, shadowY);
            xImage->refreshIcon(); 
        }
    }
    else 
    {
        shadowVisible = true;
        shadowX = x;
        shadowY = y;

        findSnapPosition(shadowX, shadowY);
        fixPosition(shadowX, shadowY);

        xImage->moveWindow(shadowX, shadowY);
        xImage->mapWindow();
        xImage->refreshIcon();
    }
}

void XIconWithShadow::dragButtonRelease(XEvent ev)
{	
    XImlib2Shadow * xImage = dynamic_cast<XImlib2Shadow *>(shadowImage);

    XIcon::dragButtonRelease(ev);
    xImage->unmapWindow();

    shadowVisible = false;
}

void XIconWithShadow::lowerIcon()
{
    XImlib2Image * xImage = dynamic_cast<XImlib2Image *>(image);
    XImlib2Shadow * sImage = dynamic_cast<XImlib2Shadow *>(shadowImage);
    xImage->lowerWindow();

    if (captionOn)
    {
	XImlib2Caption * xCaption = dynamic_cast<XImlib2Caption *>(caption);
        xCaption->lowerWindow();
    }
    sImage->lowerWindow();

}

void XIconWithShadow::findSnapPosition(int &xCord, int &yCord)
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

void XIconWithShadow::renderShadowToImage(Pixmap &buffer, int fX, int fY)
{
    XImlib2Shadow * sImage = dynamic_cast<XImlib2Shadow *>(shadowImage);
    sImage->renderShadowToImage(buffer, fX, fY);
}
