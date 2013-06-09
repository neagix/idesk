
/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2Shadow.cpp
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

#include "XImlib2Shadow.h"
#include "XIconWithShadow.h"

XImlib2Shadow::XImlib2Shadow(AbstractContainer * c, AbstractIcon * iParent,
                           AbstractConfig * con, AbstractIconConfig * iConfig)
                            : XImlib2Image(c, iParent, con, iConfig)
{
}

void XImlib2Shadow::refreshIcon()
{
	
    XIconWithShadow * sIcon = dynamic_cast<XIconWithShadow *>(iconParent);

    x = sIcon->getShadowX();
    y = sIcon->getShadowY();

    //don't need to call applyMouseOver stuff
    XImlib2Image::repaint();
}

void XImlib2Shadow::configure()
{	
    XImlib2Image::configure();

    DesktopIconConfig * dIconConfig =
        dynamic_cast<DesktopIconConfig *>(iconConfig);
    
    transparency = dIconConfig->getSnapShadowTrans();

    if (transparency == -1 )
        transparency = 200; //default value

    imlib_context_set_image(image);
    imlib_context_set_color_modifier(colorMod);
    imlib_set_color_modifier_tables(mapNone, mapNone, mapNone,
                                    defaultTransTable);
    
}

void XImlib2Shadow::moveWindow(int xCord, int yCord)
{
    XDesktopContainer * xContainer =
        dynamic_cast<XDesktopContainer *>(container);
    XIcon * xIcon = dynamic_cast<XIcon *>(iconParent);

    if (xCord >= 0 &&
        yCord >= 0 &&
        xCord < xContainer->widthOfScreen() &&
        yCord < xContainer->heightOfScreen() )
    {
        x = xCord;
        y = yCord;
        XMoveWindow( xContainer->getDisplay() , window, x, y );
    }
}

void XImlib2Shadow::renderShadowToImage(Pixmap &buffer, int fX, int fY)
{
    XIconWithShadow * sIcon = dynamic_cast<XIconWithShadow *>(iconParent);
    
    //sIcon->snapShadow();

    x = sIcon->getShadowX();
    y = sIcon->getShadowY();

    int xCord = x - fX;
    int yCord = y - fY;
    
    imlib_context_set_image(image);
    imlib_context_set_anti_alias(1);  //smoother scaling
    imlib_context_set_blend(1);       //automatically blend image and background

    imlib_context_set_drawable(buffer);
    imlib_render_image_on_drawable(xCord, yCord);
}
