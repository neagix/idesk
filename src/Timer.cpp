/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- Timer.cpp
 *
 * Copyright (c) 2005, FixXxeR (avelar@gmail.com)
 * based from Timer.cc (vdesk project) by MrChuoi <mrchuoi at yahoo dot com>
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

#include "Database.h"
#include <X11/Xlib.h>
#include "Timer.h"

vector<TimerControl*> Timer::items;

Timer::Timer(AbstractContainer * con):container(con)
{
	XDesktopContainer * xContainer = dynamic_cast<XDesktopContainer *>(container);
	c = ConnectionNumber(xContainer->getDisplay());
	if( fcntl( c, F_SETFD, 1 ) == -1) {
		cerr << "* Error: Couldn't mark display connection as close-on-exec\n";
		exit(0);
	}
}

void Timer::Update()
{
	int i, s = 1000000000;
	time_t now = time(0);

	for( i=items.size()-1; i>=0; i-- ) {
		int d = items[i]->GetDelay( now );
		if( d<=0 ) {
			items[i]->OnTime();
			if( !items[i]->IsOneShot() )
				d = items[i]->Reset();
			else {
				items.erase( items.begin() + i );
				continue;
			}
		}
		if( s>d ) s = d;
	}
	if( !s || s==1000000000 ) {
		usleep( 100 );
		return;
	}
	
	fd_set rfds;
	timeval tv = { s, 0 };
	FD_ZERO( &rfds );
	FD_SET( c, &rfds );
	if( !select( c+1, &rfds, 0, 0, &tv ) ) { // timeout
		now = time(0);
		for( i=items.size()-1; i>=0; i-- ) {
			TimerControl *t = items[i];

			if( t->GetDelay( now )<=0 ) {
				t->OnTime();
				if( t->IsOneShot() )
					items.erase( items.begin()+i );
				else
					t->Reset();
			}
		}
	}
}

void Timer::Add( TimerControl *t, long d )
{
	t->SetDelay( d );
	for( int i=0; i<items.size(); i++ )
		if( t==items[i] ) return;
	items.push_back( t );
}

void Timer::Remove( TimerControl *t )
{
	for( int i=0; i<items.size(); i++ )
		if( t==items[i] ) {
			items.erase( items.begin() + i );
			return;
		}
}
