/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- DesktopConfig.h
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

#ifndef ACTION_CONFIG_CLASS
#define ACTION_CONFIG_CLASS

#include "AbstractClasses.h"
#include "Action.h"
#include "Database.h"

class ActionConfig : public AbstractActionConfig
{
    protected:

    public:
        ActionConfig(Database db, string str);
        virtual ~ActionConfig();

        virtual void setDefaults();
        virtual void setOptions(Table);
        
        void parseActionString(string actionStr, Action * action);
        Click parseClick(string str);
};

#endif
