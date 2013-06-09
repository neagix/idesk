/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XIconWithShadow.h
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

#ifndef XICONWITHSHADOW
#define XICONWITHSHADOW

#include "XIcon.h"
#include "XImlib2Shadow.h"

class XIconWithShadow : public XIcon
{
    protected:
        XImlib2Shadow * shadowImage;

        bool shadowVisible;
        int shadowX, shadowY;


    public:
        XIconWithShadow(AbstractContainer * cont, AbstractConfig * con,
              AbstractIconConfig * iConfig);
        virtual ~XIconWithShadow();

        virtual int getShadowX() { return shadowX; }
        virtual int getShadowY() { return shadowY; }
        void findSnapPosition(int &xCord, int &yCord);
        void renderShadowToImage(Pixmap &buffer, int fX, int fY);
        void snapShadow();
        void lowerIcon();
	
	bool createIcon();
        
        void dragMotionNotify(XEvent ev);
        void dragButtonRelease(XEvent ev);
};

#endif
