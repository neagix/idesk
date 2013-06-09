/* Idesk -- Action.cpp
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

#include "Action.h"

Action::Action() : shift(false), control(false), alt(false), left(none),
            middle(none), right(none) {}

Action::Action(bool s, bool c, bool a, Click l, Click m, Click r)
{
    shift = s;
    control = c;
    alt = a;

    left = l;
    middle = m;
    right = r;
}

//input what keys/mouse buttons have been pressed as the current action
bool Action::isOccuring(Action current)
{
    //Action can only work if there is at least one mouse button
    if (left == none && middle == none && right == none)
        return false;
    //else
    
    bool returnBool;

    //if shift is set to true then current.shift must be true
    //same for control and alt
    returnBool = (!shift || current.shift) &&  
                    (!control || current.control) &&
                    (!alt || current.alt);
    
    //if left is set to none, then no checking is necessary
    //if left is set to a click then current.left must be set to the
    //same or higher number of clicks
    returnBool &= ((left == none) ||
    ((left == hold) && (current.left == hold)) ||
    ((left == singleClk) && ((current.left == singleClk) ||
                                (current.left == doubleClk) ||
                                (current.left == tripleClk))) ||
    ((left == doubleClk) && ((current.left == doubleClk) ||
                                (current.left == tripleClk))) ||
    ((left == tripleClk) &&  (current.left == tripleClk)));
    
    returnBool &= ((middle == none) ||
    ((middle == hold) && (current.middle == hold)) ||
    ((middle == singleClk) && ((current.middle == singleClk) ||
                                (current.middle == doubleClk) ||
                                (current.middle == tripleClk))) ||
    ((middle == doubleClk) && ((current.middle == doubleClk) ||
                                (current.middle == tripleClk))) ||
    ((middle == tripleClk) &&  (current.middle == tripleClk)));

    returnBool &= ((right == none) ||
    ((right == hold) && (current.right == hold)) ||
    ((right == singleClk) && ((current.right == singleClk) ||
                                (current.right == doubleClk) ||
                                (current.right == tripleClk))) ||
    ((right == doubleClk) && ((current.right == doubleClk) ||
                                (current.right == tripleClk))) ||
    ((right == tripleClk) &&  (current.right == tripleClk)));

    return returnBool;
}

void Action::clear()
{
    shift = false;
    control = false;
    alt = false;
    left = none;
    middle = none;
    right = none;
}

void Action::print()
{
    cout << "Action: " << ( shift ? " shift " : "" )
                       << ( control ? " control " : "" )
                       << ( alt ? " alt " : "" )
                       << " Left = " << left
                       << " Middle = " << middle
                       << " Right = " << right << endl;
}

void Action::setButton(int button, Click c)
{
    switch (button)
    {
        case 0:
            left = c;
            break;
        case 1:
            middle = c;
            break;
        case 2:
            right = c;
            break;
    }
}

Click Action::getButton(int button)
{
    Click returnClk = left;

    switch (button)
    {
        case 0:
            returnClk = left;
            break;
        case 1:
            returnClk = middle;
            break;
        case 2:
            returnClk = right;
            break;
    }
    return returnClk;
}
