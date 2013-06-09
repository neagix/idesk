/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- DesktopIconConfig.h
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

#ifndef DESKTOP_ICON_CLASS
#define DESKTOP_ICON_CLASS

#include <fcntl.h>
#include "AbstractClasses.h"
#include "Database.h"
#include "cursors.h"    /* defines XC_watch, etc. */ 

class CommonOptions
{
    protected:
        Table table;

        string fontName, fontColor;
	
        int fontSize;
	
        bool isBold;

        string shadowColor;
        bool shadowOn;
        int shadowX, shadowY;

        int snapShadowTrans;
        bool snapShadow;

        int clickDelay;

        bool captionOnHover;
	
        string captionPlacement;
	string fillStyle;
	
	int cursorOver;

        /* TODO
         * High Contrast - I'm not that interested in this old option. I may
         * look into it if someone asks. */
 
    public:
	 CommonOptions();
        ~CommonOptions();
        virtual void setOptions(Table);
        virtual void setCommonDefaults();
        virtual void setDefaultsFromParent(CommonOptions &/***/ other);

        virtual bool getBoldness() { return isBold; }

        virtual bool isShadow() { return shadowOn; }
        virtual int getShadowX() { return shadowX; }
        virtual int getShadowY() { return shadowY; }
        virtual string getShadowColor() { return shadowColor; }

        virtual bool getSnapShadow() { return snapShadow; }
        virtual int getSnapShadowTrans() { return snapShadowTrans; }
               
        virtual string getFont() { return fontName; }
        virtual int getFontSize() { return fontSize; }
        virtual string getFontColor() { return fontColor; }
	
        virtual bool getCaptionOnHover() { return captionOnHover; }

        virtual string getCaptionPlacement() { return captionPlacement; }
	
	virtual int getCursorOver(){ return cursorOver;}
	virtual string getFillStyle(){ return fillStyle;}
};


class DesktopIconConfig : public AbstractIconConfig
{
    protected:
        CommonOptions * common;

        int x, y;
        int width, height;

    public:
	DesktopIconConfig(const string & fName, CommonOptions * parentData);
        virtual ~DesktopIconConfig();

        virtual void setIconOptions(Table);

        virtual string getExtension(const string & file);

        virtual int getX() { return x; }
        virtual int getY() { return y; }
        virtual int getWidth() { return width; }
        virtual int getHeight() { return height; }
        virtual bool isSvg() { return picExtension == "SVG"; }
        virtual bool isRaster() { return (picExtension == "JPEG" ||
                                          picExtension == "GIF"  ||
                                          picExtension == "PPM"  ||
                                          picExtension == "PGM"  ||
                                          picExtension == "XPM"  ||
	                                      picExtension == "JPG"  ||
                                          picExtension == "TIFF" ||
                                          picExtension == "PNG"); }
        virtual bool isXpm() { return picExtension == "XPM"; }

        virtual void saveIcon(int xCord, int yCord);

        virtual bool getBoldness() { return common->getBoldness(); }

        virtual bool isShadow() { return common->isShadow(); }
        virtual int getShadowX() { return common->getShadowX(); }
        virtual int getShadowY() { return common->getShadowY(); }
        virtual string getShadowColor() { return common->getShadowColor(); }

        virtual bool getSnapShadow() { return common->getSnapShadow(); }
        virtual int getSnapShadowTrans() {return common->getSnapShadowTrans();}
        
        virtual string getFont() { return common->getFont(); }
        virtual int getFontSize() { return common->getFontSize(); }
        virtual string getFontColor() { return common->getFontColor(); }
        
        virtual bool getCaptionOnHover() { return common->getCaptionOnHover(); }
		
        virtual string getCaptionPlacement() { return common->getCaptionPlacement(); }
	virtual string getFillStyle() { return common->getFillStyle();}
};

#endif
