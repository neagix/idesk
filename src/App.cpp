/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- App.h
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

#include "App.h"
#include <signal.h>
/*#include <sys/wait.h>*/

Application::Application(int arg, char ** args) : AbstractApp(arg, args)
{

    if (!processArguments())
        _exit(1);

    startIdesk();
}

Application::~Application()
{
    delete container;
}

bool Application::processArguments()
{
    bool returnBool = true;
    string tmpStr;
    
    for (int i = 0; i < argc; i++)
    {
        tmpStr = argv[i];
        if (tmpStr == "-h" || tmpStr == "-v" || tmpStr == "--help" || tmpStr == "--usage")
        {
            cout << "Idesk " << VERSION << endl << "==============\n" 
                 << "Default Prefix " << DEFAULT_PREFIX << endl << "==============\n" 
                 << "Further documentation available at:"
                 << " http://idesk.sourceforge.net\n"
                 << "\nRemember to create your ~/.ideskrc file,"
                 << " and put icons in the ~/.idesktop\ndirectory.\n";
            returnBool = false;
        }
    }
    return returnBool;
}

void signalhandler(int sig){
	if(sig == SIGCHLD){
	  int status;
	  waitpid(-1, &status, WNOHANG|WUNTRACED);
	}else{
	  _exit(1); 	
	}
}

void Application::startIdesk()
{
    // setup signals
    signal(SIGSEGV, signalhandler);
    signal(SIGFPE, signalhandler);
    signal(SIGTERM, signalhandler);
    signal(SIGINT, signalhandler);
    signal(SIGUSR1, signalhandler);
    signal(SIGUSR2, signalhandler);
    signal(SIGHUP, signalhandler);
    signal(SIGCHLD, signalhandler);
    
    container = new XDesktopContainer(this);
    if (!container)
                cout << "container is NULL\n";
    container->run();
}

void Application::restartIdesk()
{
    container->saveState();
    cout << "restarting idesk\n";
    delete container;
    
    
    //startIdesk();
    //container->create();
    //container->run();

    execvp( argv[0], argv );
}
