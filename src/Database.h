/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- Database.h
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

#ifndef Database_h
#define Database_h

#include "defaults.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class Table
{
    public:
        Table();
        Table( const string & T );
        ~Table() { }
        string Title;

        vector<string> Label;
        vector<string> Value;

        vector<string> arrayLabels;
        vector< vector<string> > arrayValues;

        bool isValid();
        void print();

        void Set( const string & L, const string & V );
        string Query( const string & L );

        void SetArray( const string & L, const string & V, int index );
        string QueryArray( const string & L, int index );
        vector<string> QueryArray( const string & L );
        bool ArrayExists( const string & L );
};

class Database
{
    private:
        string File;
        Table blankTable;

    public:
        Database();
        Database( string F );
        ~Database();

        void print();

        vector<Table> Tables;
        Table & AddTable( const string & Title );
        Table & Query( const string & Title );
        void Write( );
	void Write(const string file);
};


#endif //Database_h
