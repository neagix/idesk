/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- AbstractClasses.h
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

#ifndef ABSTRACT_IDESK_CLASSES
#define ABSTRACT_IDESK_CLASSES

#include <string>
#include <vector>
#include <iostream>
#include "Misc.h"
#include "Action.h"
#include <stdlib.h>

//class Application;  //circular include

using namespace std;

/* The classed defined below are all abstract, so they can never be
 * instantized. They represent the core functionality of idesk, and 
 * should not include any extraneous data which is specific to a certain
 * idesk configuration or usage (more on that in AbstractContainer).
 */
class AbtractApp;
class AbstractContainer;
class AbstractIcon;
class AbstractImage;
class AbstractCaption;
class AbstractConfig;
class AbstractIconConfig;
class AbstractActionConfig;

// TODO description
class AbstractApp
{
    protected:
        char ** argv;
        int argc;

        AbstractContainer * container;

        virtual bool processArguments() = 0;
    public:
        AbstractApp(int arg, char ** args) : argc(arg), argv(args) {}
        virtual ~AbstractApp() {}

        virtual void startIdesk() = 0;
        virtual void restartIdesk() = 0;
};

/* AbstractContainer
 * This class represents the holding container for the icons. It contains
 * a list of all the icons and a few methods that act on the program as a 
 * whole. Subclass this is create different icon containers. Possibilities
 * for containers are a desktop container (already implemented), a widget
 * container (gtk, qt, or other), or a taskbar container.
 */
class AbstractContainer 
{
    protected:
        vector<AbstractIcon *> iconList;
        AbstractConfig * config;
        AbstractActionConfig * actionConfig;

        AbstractApp * app;
        Action currentAction;

    public:
        AbstractContainer(AbstractApp * a) : app(a) {}
	virtual ~AbstractContainer() {} 

        virtual void run() = 0;
        virtual void destroy() = 0;
        virtual void create() = 0;
        virtual void addIcon(AbstractIcon *) = 0 ;
        virtual void eventLoop() = 0 ;

        virtual void configure() = 0;

        virtual void saveState() = 0 ;
        virtual void saveIcon(AbstractIcon *) = 0 ;
        virtual void reloadState() = 0 ;

        virtual void loadIcons() = 0 ;
        virtual void arrangeIcons() = 0 ;

        virtual void runCommand(const string & command) = 0 ;
};

/*
 * AbstractIcon
 * This class represents all the functionality of an icon. It includes an
 * image and a caption, which are the two defining characteristics of an 
 * idesk icon. Methods include functions that getcommon data used in both
 * the image and caption and methods that utilize the entire icon object
 * such as saving. Included are pointers to the parent container, the general
 * configuration class, and the configuration class specific to the icon.
 * Subclass this to create different types of icons. Possibilities include
 * icons that utilize different different drawing toolkits (Xlib, Gdk, Qt) and
 * icons that have special and/or different effects.
 */
class AbstractIcon 
{
    protected:
        AbstractContainer * container;

        AbstractImage * image;
        AbstractCaption * caption;

        AbstractConfig * config;
        AbstractIconConfig * iconConfig;

        vector<string> commandArray;
        string iconFilename;

        bool dragging, dragged;
        bool captionOn;
	bool valid;

        int x, y;

    public:
        AbstractIcon(AbstractContainer * c, AbstractConfig * con,
                AbstractIconConfig * iConfig)
            : container(c), config(con), iconConfig(iConfig) {}
        virtual ~AbstractIcon() {} ;

        virtual bool setImage(AbstractImage *) = 0 ;
        virtual bool setCaption(AbstractCaption *) = 0 ;

        virtual void mouseOverEffect() = 0 ;
        virtual void mouseOffEffect() = 0 ;

        virtual int getX() = 0;
        virtual int getY() = 0;
        virtual int getHeight() = 0;
        virtual int getWidth() = 0;
	
	virtual bool createIcon() = 0;
	virtual bool isValid(){ return valid; }
	virtual void getCursorOver(){}
	virtual void getCursorOut(){}
	virtual void event_enter_notify (){}
	virtual void event_leave_notify (){}

        virtual vector<string> getCommandArray() { return commandArray; }
        virtual string getCommand(int i) { return commandArray[i]; }

        virtual void save() = 0;
};

/* AbstractImage
 * This class represents the image functionality of an image that represents
 * the icon. Methods included perform basic image functions, such as drawing
 * loading picture data, and image state changes. Pointers are included that
 * point to the parent container, parent icon, general configuration, and
 * icon specific configuration. Subclass this class to create different kinds
 * of images. Possibilities include different image types (raster vs vector)
 * and special effect images such as a shadow or a glow.
 */
class AbstractImage 
{
    protected:
        AbstractContainer * container;
        AbstractIcon * iconParent;

        AbstractConfig * config;
        AbstractIconConfig * iconConfig;
        int width, height;
	   
	bool valid;

    public:
        AbstractImage(AbstractContainer * c, AbstractIcon * iParent,
                AbstractConfig * con, AbstractIconConfig * iConfig)
            : container(c), iconParent(iParent), config(con),
        iconConfig(iConfig) {}
        virtual ~AbstractImage() {} ;

        virtual void draw() = 0;
	virtual void redraw() {};
	
        virtual void mouseOverEffect() {}
        virtual void mouseOffEffect() {}

        int getWidth() { return width; }
        int getHeight() { return height; }
	virtual void event_enter_notify (){}
	virtual void event_leave_notify (){}

        virtual void configure() {} ;
        virtual void createPicture() = 0;
};

/*
 * AbstractCaption
 * This class represents the text functionality of an icon. The caption for 
 * an icon is only optional. Methods included perform basic manipulation of 
 * the caption window, such as drawing, loading/creating font, and changing
 * state. Pointers are included that point to the parent container, parent
 * icon, general configuration, and icon specific configuration. Subclass
 * this to create different caption types. Possibilities include captions
 * that render using different font libraries (pango, Xft, Xlib).
 */
class AbstractCaption 
{
    protected:
        AbstractContainer * container;
        AbstractIcon * iconParent;

        AbstractConfig * config;
        AbstractIconConfig * iconConfig;

        string text;

    public:
        AbstractCaption(AbstractContainer * c, AbstractIcon * iParent,
                AbstractConfig * con, AbstractIconConfig * iConfig)
            : container(c), iconParent(iParent), config(con),
        iconConfig(iConfig) {}
        virtual ~AbstractCaption() {} ;

        virtual void setText(const string & str) { text = str; }
        virtual string getText() { return text; }

        virtual int getFontWidth() = 0 ;
        virtual int getFontHeight() = 0 ;

        virtual void draw() = 0;
        //virtual void updateText() = 0;
        virtual void mouseOverEffect() {}
        virtual void mouseOffEffect() {}

        virtual void configure() = 0;
        virtual void createFont() = 0;
};

/* AbstractConfig
 * This class represents the main configuration for idesk. This class is
 * created once by the container upon initialization. This should parse
 * any configuration files and set default values. Contained is a list of 
 * configurations for each individual icon. Methods in the class allow for
 * iterator style access to the list of icon configurations and general
 * program configuration. Subclass this to create different ways to configure
 * idesk, basically for different configuration syntaxes.
 */
class AbstractConfig
{
    protected:
        vector<AbstractIconConfig *> iconConfigList;

        string ideskrcFile;

        unsigned int iterCtr;

        int clickDelay;

    public:
        AbstractConfig(string str) : ideskrcFile(str), iterCtr(0),
                                     clickDelay(300) {}
        virtual ~AbstractConfig() {}

        virtual int getNumIcons() { return iconConfigList.size(); }
        virtual AbstractIconConfig * nextIcon()
        {
            // TODO - use try and catch here to grab out-of-bounds errors
            return iconConfigList[++iterCtr];
        }
        virtual bool notFinished() { return iterCtr < iconConfigList.size(); }

        virtual AbstractIconConfig * start()
        {
            iterCtr = 0;
            return iconConfigList.front();
        }
        virtual AbstractIconConfig * end()
        {   
            iterCtr = iconConfigList.size();
            return iconConfigList.back();
        }
        virtual unsigned int numIcons() { return iconConfigList.size(); }
        virtual unsigned int getClickSpeed() { return clickDelay; }
};

/* AbstractIconConfig
 * This class is the configuration for a specific icon. This class should only
 * be created and accessed through a class that extends AbstractConfig.
 * This class pasrses data for a specific icon, whatever format that may be.
 * Members include all the options and data for an icon such as the caption 
 * and command text, picture and icon filename. It is assumed that each
 * icon has a seperate configuration file. Subclass this to create different
 * ways to configure  an icon, basically for different configuration syntaxes.
 */
class AbstractIconConfig
{
    protected:
        string iconFilename;
        string pictureFilename;
        string picExtension;
        string caption;
	string captionTip;
        vector<string> commandArray;

        int width, height;

    public:
        virtual ~AbstractIconConfig() {}

        virtual int getWidth() { return width; }
        virtual int getHeight() { return height; }

        virtual string getIconFilename() { return iconFilename; }
        virtual string getPictureFilename() { return pictureFilename; }
        virtual string getCaption() { return caption; }
	virtual string getCaptionTip() { return captionTip; }
        virtual string getCommand(int i) { return commandArray[i]; }
        virtual vector<string> getCommandArray() { return commandArray; }
};

/* AbstractActionConfig
 * Desc: TODO
 *
 */
class AbstractActionConfig
{
    protected:
        vector<Action *> executeActions;
        Action * lock;
        Action * reload;
        Action * drag;
        Action * endDrag;

        string ideskrcFile;

    public:
        AbstractActionConfig(string str) : ideskrcFile(str) {}
        virtual ~AbstractActionConfig() {}

        // returning an dynamically allocated pointer, so some other class must
        // do unallocation
        virtual Action * getLock() { return lock; }
        virtual Action * getReload() { return reload; }
        virtual Action * getDrag() { return drag; }
        virtual Action * getEndDrag() { return endDrag; }

        virtual vector<Action *> getExecuteActions() { return executeActions; }
        virtual Action * getExecuteAction(int i) { return executeActions[i]; }
};
        

#endif
