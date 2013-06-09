/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- App.cpp
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

#ifndef APP_CLASS
#define APP_CLASS

#include "Misc.h"
#include "config.h"
#include "defaults.h"
#include <iostream>
#include "AbstractClasses.h"

#include "XDesktopContainer.h"
//circular including
//class XDesktopContainer;  // .h file is below

class Application : public AbstractApp
{    
	
    protected:
        AbstractContainer * container;
        bool processArguments();
    public:
        Application(int arg, char ** args);
        virtual ~Application();
        void startIdesk();
        void restartIdesk(); 
};


#endif
