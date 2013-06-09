/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- Timer.h
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

#ifndef  TIMER_CLASS
#define  TIMER_CLASS

#include <time.h>
#include "XDesktopContainer.h"
#include <fcntl.h>

class TimerControl {
private:
	long start;
	long delay;
public:
	void SetDelay( long d ) { delay = d; start = time(0); }
	long GetDelay( long n ) { return start + delay - n; }
	void Finish() { start = delay;}
	long Reset() { start = time(0); return delay; }

	virtual bool IsOneShot() { return true; }
	virtual void OnTime() = 0;
};

class Timer{
private:
	static vector<TimerControl*> items;
         AbstractContainer * container;
	int c;
public:
	Timer(AbstractContainer * con);
	void Update();
public:
	static void Add( TimerControl *t, long d );
	static void Remove( TimerControl *t );
};

#endif
