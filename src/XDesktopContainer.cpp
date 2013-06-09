/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- XDesktopContainer.cpp
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

#include "XDesktopContainer.h"
#include "XIconWithShadow.h"
#include "XImlib2Background.h"
#include "Database.h"


#include <X11/keysym.h>
#ifdef HAVE_STARTUP_NOTIFICATION
#include <libsn/sn.h>
#endif /* HAVE_STARTUP_NOTIFICATION  */

XDesktopContainer *xcontainer;

XDesktopContainer::XDesktopContainer(AbstractApp * a) : DesktopContainer(a)
{
    xcontainer=this; 	
    initXWin();
    initImlib();
}

void XDesktopContainer::run()
{
    times[0] = 0; times[1] = 0; times[2] = 0; 
    numClicks[0] = 0; numClicks[1] = 0; numClicks[2] = 0; 
    configure();
    create();
    loadIcons();
    arrangeIcons();

    eventLoop();
}

XDesktopContainer::~XDesktopContainer()
{
    destroy();
    XCloseDisplay(display);
}

void XDesktopContainer::create()
{
    getRootImage();
}

void XDesktopContainer::destroy()
{
    
    vector<AbstractIcon *>::reverse_iterator rIt = iconList.rbegin();
    for(; rIt != iconList.rend(); rIt++)
        delete *rIt;

    iconList.clear();
        
    delete config;
    delete actionConfig;
    delete bg;
    
    XFlush(display);
}

void XDesktopContainer::initXWin()
{
	
    display = XOpenDisplay(NULL);
    
    if (!display){
	 cout << "Display is null!!\n";
	 _exit(1);
    }
    
     char *name =  DisplayString(display);
     int screen = DefaultScreen(display);
     rootWindow  = RootWindow(display,  screen);
     
     XTextProperty prop;
     Atom start = XInternAtom(display,"_IDESK_START", false);
     cout << " Idesk starting in " <<  name << endl;
     prop.value = (unsigned char *)name;
     prop.encoding = XA_STRING;
     prop.format = 8;
     prop.nitems = strlen(name);
     XSetTextProperty(display, rootWindow, &prop, start);
     
     XSelectInput( display, rootWindow,  PropertyChangeMask| SubstructureNotifyMask);
     XSync(display, false);
     
}

void XDesktopContainer::initImlib()
{
    //Imlib2 stuff
    imlib_context_set_display(display);
    imlib_context_set_visual(DefaultVisual(display,DefaultScreen(display)) );
    imlib_context_set_colormap(DefaultColormap(display, 
                DefaultScreen(display)) );
}

void XDesktopContainer::getRootImage()
{
     DesktopConfig * dConfig = dynamic_cast<DesktopConfig *>(config);
     
     
     imlib_context_set_drawable(rootWindow);
     
     bg = new XImlib2Background(this,config);
    
     bg->InitSpareRoot(rootWindow);
	
     if(!bg->IsOneShot()){	
        timer = new Timer(this);
	bg->Finish();
     }
}

void XDesktopContainer::configure()
{    
    //get the user's config file
    string ideskrcFile = getenv("HOME");
    ideskrcFile += "/.ideskrc";

    Database db(ideskrcFile);
    config = new DesktopConfig(db, ideskrcFile);
    
    DesktopConfig * dConfig = dynamic_cast<DesktopConfig *>(config);
    
    if(config->numIcons() == 0){
	    dConfig->loadDefaultIcons();
    }

    locked = dConfig->getLocked();
    clickDelay = dConfig->getClickSpeed();

    snapState = dConfig->getSnapState();
    snapShadow = dConfig->getSnapShadow();
    snapWidth = dConfig->getSnapWidth();
    snapHeight = dConfig->getSnapHeight();

    actionConfig = new ActionConfig(db, ideskrcFile);
}   

void XDesktopContainer::loadIcons()
{
    AbstractIconConfig * iconPtr;

    if (config->numIcons() == 0)
    {
        cout << "No icons loaded!! .idesktop is empty or contains invalid icons\n";
        _exit(1);
    }
    else
    {
        //iterate through all the icons created by the configure class
        for(iconPtr = config->start(); config->notFinished();
            iconPtr = config->nextIcon())
        {
            DesktopIconConfig * dIconConfig =
                dynamic_cast<DesktopIconConfig *>(iconPtr);

            XIcon * icon;
            if (dIconConfig->getSnapShadow() && dIconConfig->getSnapShadow()){
                icon = new XIconWithShadow(this, config, iconPtr);
	    }
            else{
                icon = new XIcon(this, config, iconPtr);
		
	}
	if (icon->isValid()){
		if(icon->createIcon()){
	    		   addIcon(icon);
		}
	}
      }
    }
}

void XDesktopContainer::arrangeIcons()
{
    int maxW = 0;
    int iconX, iconY = 20;

    if( iconList.size() == 0 )
    {
        cout << "No Icons! Quitting.\n";
        _exit(1);
    }

    for(unsigned int i = 0; i < iconList.size(); i++ )
    {
        XIcon *iPtr = dynamic_cast<XIcon *>(iconList[i]);
        if( iPtr->getWidth() > maxW )
            maxW = iPtr->getWidth();
    }

    iconX = widthOfScreen() - maxW - 20;

    for(unsigned int i = 0; i < iconList.size(); i++ )
    {
        XIcon *iPtr = dynamic_cast<XIcon *>(iconList[i]);

        if( iconY + iPtr->getHeight() + 30 + iPtr->getFontHeight() >
                heightOfScreen() )
        {
            iconY = 20;
            iconX = iconX - 20 - maxW;
        }
        
        if( iPtr->getX() == 0 && iPtr->getY() == 0 )
        {
            iPtr->setX(iconX + ((maxW - iPtr->getWidth())/2));
            iPtr->setY(iconY);
            iconY += iPtr->getHeight() + 30 + iPtr->getFontHeight();
        }
        
        iPtr->moveImageWindow();
        iPtr->mapImageWindow();
        //don't initially map caption for the hover effect
        iPtr->initMapCaptionWindow();
        
        //iPtr->draw();
         
    }
}

void XDesktopContainer::addIcon(AbstractIcon * icon)
{
    iconList.push_back(icon);
}

XIcon * XDesktopContainer::findIcon(Window window)
{
    for(unsigned int i = 0; i < iconList.size(); i++)
    {
        XIcon * tmpIcon = dynamic_cast<XIcon *> (iconList[i]);
        Window * tmpCapWindow = tmpIcon->getCaptionWindow();
        if ( *tmpIcon->getImageWindow() == window ||
             (tmpCapWindow != NULL && *tmpCapWindow == window) )
            return tmpIcon;
    }
    return None;
}

void XDesktopContainer::updateIcons()
{
	for(unsigned int i = 0; i < iconList.size(); i++)
	{
		XIcon * tmpIcon = dynamic_cast<XIcon *> (iconList[i]);
		tmpIcon->draw();
	}
}
    
void XDesktopContainer::eventLoop()
{
    XEvent ev;
#ifdef HAVE_STARTUP_NOTIFICATION
    sn_context = NULL;
    sn_display = NULL; 
    sn_bool_t retval;
    sn_display = sn_display_new (display,
        		         error_trap_push,
				 error_trap_pop);

#endif /* HAVE_STARTUP_NOTIFICATION  */
    
    for(;;)
    {
        if( !XPending( display ) && timer){
		if(!bg->IsOneShot()){
			timer->Update();
		}
	}
	else {
	 
          XNextEvent(display, &ev);
#ifdef HAVE_STARTUP_NOTIFICATION
	  if (sn_display != NULL){
	   sn_display_process_event (sn_display, &ev);
          }
#endif /* HAVE_STARTUP_NOTIFICATION  */
          event = ev;
          parseEvent();
	}
    }
    
#ifdef HAVE_STARTUP_NOTIFICATION
    sn_launcher_context_unref (sn_context);
    if (sn_display)
    {
       sn_display_unref (sn_display);
    }
#endif /* HAVE_STARTUP_NOTIFICATION  */
}

void XDesktopContainer::parseEvent()
{
    currentAction.clear();

    parseNonIconEvents();
    XIcon * icon = parseIconEvents();

    exeCurrentAction(icon);
}

void XDesktopContainer::parseNonIconEvents()
{
	
    switch (event.type)
    {
	    
        case PropertyNotify:
		//char *name = XGetAtomName(display, event.xproperty.atom );
		//cout << " Name " << name << endl ;
		
		static Atom atom_stop = None ;
                if( atom_stop == None ) atom_stop = XInternAtom(display, "_IDESK_STOP", True);
		
		if (event.xproperty.atom == atom_stop && !stop){
			XTextProperty prop; 
			int result = XGetTextProperty(display, rootWindow, &prop, atom_stop);
			if(result && prop.encoding != None && prop.value != NULL){
				string current_display_name = DisplayString(display);
				string old_display_name = (char *)prop.value;
				string mesg = (char *)prop.value;
				if(current_display_name == old_display_name){
					XDeleteProperty (display, rootWindow, atom_stop);
					cout << "Error ... Idesk is running in " << prop.value << endl;
					cout << "Exit." << endl;
					_exit(1);
				}
			}
		}
		
		static Atom atom_start = None ;
                if( atom_start == None ) atom_start = XInternAtom(display, "_IDESK_START", True);
		
		if (event.xproperty.atom == atom_start){
		        XTextProperty prop; 
			int result = XGetTextProperty(display, rootWindow, &prop, atom_start);
			if(result && prop.encoding != None && prop.value != NULL){
				string current_display_name = DisplayString(display);
				string old_display_name = (char *)prop.value;
				if(current_display_name == old_display_name){
					XDeleteProperty (display, rootWindow, atom_start);
					stop = XInternAtom(display,"_IDESK_STOP", false);
					prop.value = (unsigned char *)current_display_name.c_str();
					prop.encoding = XA_STRING;
					prop.format = 8;
					prop.nitems = strlen(current_display_name.c_str());
					XSetTextProperty(display, rootWindow, &prop, stop);
				}
			}
		}
		
		static Atom atom_xroot = None ;
                if( atom_xroot == None ) atom_xroot = XInternAtom(display, "_XROOTPMAP_ID", True);
		
	        if (event.xproperty.atom == atom_xroot)
	         {
			Pixmap pmap = bg->GetRootPixmap(event.xproperty.atom);
			if(bg->pixmap == None){
				bg->Refresh(pmap);
				bg->pixmap = (Pixmap)1; //For Fix
			}else{
				bg->InitSpareRoot(event.xproperty.window);
			}
			updateIcons(); 
		 }
		 break;
    }
}

XIcon * XDesktopContainer::parseIconEvents()
{
    XIcon * icon;

    icon = findIcon(event.xmotion.window);
   
    if (icon)
    {
        switch (event.type)
        {
            case ButtonPress:
                setEventState();

                if (event.xbutton.button == Button1)
                    currentAction.setLeft(hold);                
                else if (event.xbutton.button == Button2)
                    currentAction.setMiddle(hold);
                else if (event.xbutton.button == Button3)
		             currentAction.setRight(hold);
		
                if(event.xbutton.window == *icon->getImageWindow()  || event.xbutton.window == *icon->getCaptionWindow()){
			 if(bg->spareRoot){
				 icon->pressImage();
			 }
		}
                        
                break;
			
            case MotionNotify:
                if (icon->isDragging() && !isLocked())
		     icon->dragMotionNotify(event);
                break;

            case ButtonRelease:
                setEventState();

                if (event.xbutton.button == Button1)
                    translateButtonRelease(0);
                else if (event.xbutton.button == Button2)
                    translateButtonRelease(1);
                else if (event.xbutton.button == Button3)
		            translateButtonRelease(2);

		if(event.xbutton.window == *icon->getImageWindow() || event.xbutton.window == *icon->getCaptionWindow()){
			 if(bg->spareRoot){
				 icon->unpressImage();
			 }
	        }

                break;
			    
            case Expose:
                //since we are redrawing the whole window we can ignore
                //multiple expose events and only draw text once
		 if (event.xexpose.count == 0){
			 if(bg->spareRoot){
                      		icon->draw();	
			 }  
		  }
                break;
            case EnterNotify:
	        if(event.xcrossing.window == *icon->getImageWindow() || event.xcrossing.window == *icon->getCaptionWindow()){
			if(bg->spareRoot){
				icon->mouseOverEffect();
				icon->event_enter_notify();
			}
		 }
                break;
            case LeaveNotify:  
	        if(event.xcrossing.window == *icon->getImageWindow() || event.xcrossing.window == *icon->getCaptionWindow()){
			if(bg->spareRoot){
				icon->mouseOffEffect();
				icon->event_leave_notify();
			}
		}
                break;     
        }
    }
    return icon;
}

void XDesktopContainer::exeCurrentAction(XIcon * icon)
{
			
	if (actionConfig->getReload()->isOccuring(currentAction)){
		app->restartIdesk();
	}
    
    if (actionConfig->getLock()->isOccuring(currentAction))
    {
        toggleLock();
        DesktopConfig * dConfig = dynamic_cast<DesktopConfig *>(config);
        dConfig->saveLockState(locked); 
    }
    
    if (icon) //make sure icon is not NULL
    {
        if (actionConfig->getDrag()->isOccuring(currentAction)
            && !isLocked()
            && !icon->isDragging() ) //only start drag if not already occuring
            icon->dragButtonPress(event);
        else if (actionConfig->getEndDrag()->isOccuring(currentAction))
            icon->dragButtonRelease(event);

        for (int i = 0; i < icon->getCommandArray().size() &&
                        i < actionConfig->getExecuteActions().size();
                        i++)
		if (actionConfig->getExecuteAction(i)->isOccuring(currentAction)){
#ifdef HAVE_STARTUP_NOTIFICATION
			if (sn_display != NULL)
			{
			  sn_context = sn_launcher_context_new (sn_display, DefaultScreen (display));
			  if ((sn_context != NULL) && !sn_launcher_context_get_initiated (sn_context))
			  {
			   sn_launcher_context_set_name (sn_context, icon->getCommand(i).c_str());
			   sn_launcher_context_set_description (sn_context, icon->getCommand(i).c_str());
		           sn_launcher_context_set_binary_name (sn_context, icon->getCommand(i).c_str());
                           sn_launcher_context_set_icon_name(sn_context, icon->getCommand(i).c_str());
			   
			   sn_launcher_context_initiate (sn_context,
					   icon->getCommand(i).c_str(),
					   icon->getCommand(i).c_str(),	      
					   event.xproperty.time);
			  }
			}
#endif  /*HAVE_STARTUP_NOTIFICATION */
			runCommand(icon->getCommand(i));
		}
    }

}

void XDesktopContainer::setEventState()
{
    currentAction.setControl(false);
    currentAction.setShift(false);
    currentAction.setAlt(false);

    if (event.xbutton.state & ControlMask) currentAction.setControl(true);
    if (event.xbutton.state & ShiftMask) currentAction.setShift(true);
    if (event.xbutton.state & Mod1Mask) currentAction.setAlt(true);

    if (event.xbutton.state & Button1Mask && currentAction.getLeft() == none)
        currentAction.setLeft(hold);

    if (event.xbutton.state & Button2Mask && currentAction.getMiddle() == none)
        currentAction.setMiddle(hold);

    if (event.xbutton.state & Button3Mask && currentAction.getRight() == none)
        currentAction.setRight(hold);
}
    
void XDesktopContainer::translateButtonRelease(int button)
{
    if (event.xbutton.time - times[button] <= clickDelay)
        numClicks[button]++;
    else {
        numClicks[button] = 1;
        times[button] = event.xbutton.time;
    }

    if (numClicks[button] == 1)
        currentAction.setButton(button, singleClk);
    else if (numClicks[button] == 2)
        currentAction.setButton(button, doubleClk);
    else if (numClicks[button] >= 3)
        currentAction.setButton(button, tripleClk);
    else
        currentAction.setButton(button, none);
}

void XDesktopContainer::saveState()
{
    //save each of the icons
    for(unsigned int i = 0; i < iconList.size(); i++)
        saveIcon(iconList[i]);

    //general config saves
    
    DesktopConfig * dConfig = dynamic_cast<DesktopConfig *>(config);

    dConfig->saveLockState(locked);
}

void XDesktopContainer::saveIcon(AbstractIcon * xIcon)
{
    xIcon->save();
}

void XDesktopContainer::reloadState()
{
    //TODO -- Reload all of the icons internally instead of rebooting whole
    //        program. Not way too important though.
}

void XDesktopContainer::runCommand(const string & command)
{
     pid_t pid;   
    //fork and execute program
    if (pid=fork() != 0) { //Primer proceso hijo
#ifdef HAVE_STARTUP_NOTIFICATION
	    if (sn_context != NULL)
		     sn_launcher_context_setup_child_process (sn_context);
#endif /* HAVE_STARTUP_NOTIFICATION  */
                setsid();
		if(execl("/bin/sh", "/bin/sh", "-c", command.c_str(), 0) == -1){
			printf("Error to execute command %s\n", command.c_str());
			exit(1);
		}
		exit(0); //exit fork
    }
    waitpid(pid, NULL, 0);
}
int XDesktopContainer::widthOfScreen()
{
    return WidthOfScreen(DefaultScreenOfDisplay(display));
}

int XDesktopContainer::heightOfScreen()
{
    return HeightOfScreen(DefaultScreenOfDisplay(display));
}

