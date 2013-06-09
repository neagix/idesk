/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- DesktopContainer.h
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

#ifndef DESKTOP_CONTAINER_CLASS
#define DESKTOP_CONTAINER_CLASS

#include "AbstractClasses.h"

class DesktopContainer : public AbstractContainer
{
    protected:
        int snapWidth;
        int snapHeight;
        bool snapState, snapShadow;
        bool startSnapLeft, startSnapTop;
        
        int destopWidth, destopHeight;

        unsigned int clickDelay;

        bool locked;

    public:
        DesktopContainer(AbstractApp * a) : AbstractContainer(a) {}
        virtual ~DesktopContainer() {}

        virtual void lockDesktop() { locked = true; }
        virtual void unlockDesktop() { locked = false; }
        virtual bool isLocked() { return locked; }
        virtual void toggleLock() { locked = locked ? false : true; }

        virtual void setSnap(int xVal, int yVal) { snapWidth = xVal;
                                                   snapHeight = yVal; }
        virtual void snapOff() { snapState = false; }
        virtual void snapOn() { snapState = true; }
        virtual void toggleSnap() { snapState = snapState ? false : true; }

        virtual unsigned int getClickSpeed() { return clickDelay; }
};

#endif
