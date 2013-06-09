/* Idesk -- Action.h
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

#ifndef ACTION_CLASS
#define ACTION_CLASS

#include <iostream>

using namespace std;

enum Click { none, singleClk, doubleClk, tripleClk, hold };

class Action
{
    protected:
        //keyboard modifiers
        bool shift, control, alt;

        //mouse modifiers
        Click left, middle, right;

        //other modifiers - Implemented later
        //int clickDelay;
        
    public:
        Action();
        Action(bool s, bool c, bool a, Click l, Click m, Click r);
        virtual ~Action() {}

        virtual void clear();
        virtual void print();
        
        virtual bool isOccuring(Action current);
        
        virtual void setShift(bool s) { shift = s; }
        virtual void setControl(bool c) { control = c; }
        virtual void setAlt(bool a) { alt = a; }

        virtual void setLeft(Click l) { left = l; }
        virtual void setMiddle(Click m) { middle = m; }
        virtual void setRight(Click r) { right = r; }

        virtual void setButton(int button, Click c);
        virtual Click getButton(int button);

        virtual bool getShift() { return shift; }
        virtual bool getControl() { return control; }
        virtual bool getAlt() { return alt; }

        virtual Click getLeft() { return left; }
        virtual Click getMiddle() { return middle; }
        virtual Click getRight() { return right; }
};

#endif
