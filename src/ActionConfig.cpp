/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- DesktopConfig.cpp
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

#include "ActionConfig.h"

ActionConfig::ActionConfig(Database db, string str) :
                          AbstractActionConfig(str)
{
    Table table = db.Query("Actions");

    if (!table.isValid())
    {
        cout << "Can't find config file or missing 'Actions' table in the "
             << "config file!!\n";
        _exit(-1);
    }
    
    setOptions(table);
}

ActionConfig::~ActionConfig()
{
    delete lock;
    delete reload;
    delete drag;
    delete endDrag;

    for (int i = 0; i < executeActions.size(); i++)
        delete executeActions[i];

}

void ActionConfig::setOptions(Table table)
{
    setDefaults();

    if (table.Query("Reload") != "")
        parseActionString(table.Query("Reload"), reload);

    if (table.Query("Lock") != "")
        parseActionString(table.Query("Lock"), lock);
    
    if (table.Query("Drag") != "")
        parseActionString(table.Query("Drag"), drag);

    if (table.Query("EndDrag") != "")
        parseActionString(table.Query("EndDrag"), endDrag);
    
    if (table.ArrayExists("Execute"))
    {
        vector<string> exeValues = table.QueryArray("Execute");

        //first execute action is already allocated
        parseActionString(exeValues[0], executeActions[0]);
        
        for(int i = 1; i < exeValues.size(); i++)
        {
            executeActions.push_back(new Action(false, false, false, none, none,
                                                none));
            parseActionString(exeValues[i], executeActions[i]);
        }
    }
}

void ActionConfig::setDefaults()
{
    // Action(shift, control, alt, left, middle, right);
    reload = new Action(false, false, false, none, doubleClk, none);
    lock = new Action(false, false, false, none, none, doubleClk);
    drag = new Action(false, false, false, hold, none, none);
    endDrag = new Action(false, false, false, singleClk, none, none);

    Action * exeAction = new Action(false, false, false, doubleClk, none, none);
    executeActions.push_back(exeAction);
}

void ActionConfig::parseActionString(string actionStr, Action * action)
{
 
    stringstream ss(actionStr, stringstream::in | stringstream::out);

    string tmpStr;

    action->setShift(false);
    action->setControl(false);
    action->setAlt(false);
    action->setLeft(none);
    action->setMiddle(none);
    action->setRight(none);
    
    while (!ss.eof())
    {
        ss >> tmpStr;
        tmpStr = getUpper(tmpStr);
        
        if (tmpStr == "SHIFT")
            action->setShift(true);
        else if (tmpStr == "CONTROL")
            action->setControl(true);
        else if (tmpStr == "ALT")
            action->setAlt(true);
        else if (tmpStr == "LEFT") {
            ss >> tmpStr;
            action->setLeft(parseClick(tmpStr));
        }
        else if (tmpStr == "MIDDLE") {
            ss >> tmpStr;
            action->setMiddle(parseClick(tmpStr));
        }
        else if (tmpStr == "RIGHT") {
            ss >> tmpStr;
            action->setRight(parseClick(tmpStr));
        }
    }
}

Click ActionConfig::parseClick(string str)
{
    Click returnClk=singleClk;

    str = getUpper(str);
    
    if (str == "SINGLECLK")
        returnClk = singleClk;
    else if (str == "DOUBLECLK")
        returnClk = doubleClk;
    else if (str == "TRIPLECLK")
        returnClk = tripleClk;
    else if (str == "HOLD")
        returnClk = hold;

    return returnClk;
}
