/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XImlib2Shadow.h
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

#ifndef X_IMLIB2_SHADOW_H
#define X_IMLIB2_SHADOW_H

#include "XImlib2Image.h"

class XImlib2Shadow : public virtual XImlib2Image
{
    public:
        XImlib2Shadow(AbstractContainer * c, AbstractIcon * iParent,
                       AbstractConfig * con, AbstractIconConfig * iConfig);
        virtual ~XImlib2Shadow() {}

        virtual void configure();

        virtual void moveWindow(int xCord, int yCord);
        virtual void refreshIcon();
        virtual void renderShadowToImage(Pixmap &buffer, int fX, int fY);
};

#endif
