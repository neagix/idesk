/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- Misc.cpp
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

#include "Misc.h"

extern char ** args;

string getUpper(const string & str)
{
    string work = str;
    transform(work.begin(), work.end(), work.begin(),(int(*)(int)) toupper);
    return work;
}
    
/*void Reboot()
{
    execvp( args[0], args );
}*/

string itos(int i) // convert int to string
{
 stringstream s;
 s << i;
 return s.str();
}
