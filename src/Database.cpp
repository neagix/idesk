/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- Database.cpp
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

#include "Database.h"
#include "Util.h"
#include "Misc.h"

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

Database::Database( string  F, bool fallbackToDefaultRc ) : blankTable()
{
    wasLoaded = false;
    ifstream DbFile;
    DbFile.open(F.c_str(), ios::in);
    
    if (!DbFile.is_open()) {
        if (!fallbackToDefaultRc) {
            cout << "Cannot read file: " << F << "\n";
            _exit(1);
        }

        cout << "Cannot read file: " << F << "\nUsing embedded defaults\n";

        //TODO: add all defaults here
        {
        Table & T = AddTable( "Config" );
        T.Set("FontName", "Arial");
        T.Set("FontSize", "10");
        T.Set("FontColor", "#000000");
        }

        {
        Table & T = AddTable( "Actions" );
        T.Set("Lock", "control right doubleClk");
        T.Set("Reload", "middle doubleClk");
        T.Set("Drag", "left hold");
        T.Set("EndDrag", "left singleClk");
        T.SetArray("Execute", "left doubleClk", 0);
        T.SetArray("Execute", "right doubleClk", 1);
        }

        return;
    }

    string Buffer, Line, temp;

    File = F;

    while( DbFile ) {
        DbFile >> Buffer;
        if( Buffer == "table" ) {

            getline( DbFile, Line );
            Line = Line.substr( 1, Line.size()-1 );
            Table & T = AddTable( Line );

            while( DbFile ) {
                DbFile >> Buffer;
                
                //end of table
                if( Buffer == "end" ) break;
                //array value
                else if (Buffer[Buffer.size()-1] == ':' &&
                         Buffer.size() >= 2 &&  //don't overstep array
                         Buffer[Buffer.size()-2] == ']')
                {
                    //grab the index contained in '[' and ']'
                    int lBIndex = Buffer.find('[', 0);
                    temp = Buffer.substr(lBIndex + 1,
                                         Buffer.size() - lBIndex - 3);
                    int index = atoi(temp.c_str());

                    //grab label as everything before the '['
                    Buffer = Buffer.substr(0, lBIndex);

                    getline( DbFile, Line );
                    Line = Line.substr( 1, Line.size()-1 );
                    T.SetArray( Buffer, Line, index );
                }
                //regular value
                else if( Buffer[Buffer.size()-1] == ':' ) {
                    Buffer = Buffer.substr( 0, Buffer.size()-1 );
                    getline( DbFile, Line );
		    //cout << "Buffer:'" << Buffer	<< "'" << endl;
			if (Line.size()>0){
				Line = Line.substr( 1, Line.size()-1 );
/*				if(getUpper(Buffer) == "ICON")
				{
					//remove all white spaces to filename
					int pos = Line.find(' ', 0); 
					if( pos != string::npos){ //search for white spaces from the end
						Line =   Line.substr( 0 , pos); 
					}
					cout << "Line:'"<< Line <<"'" <<endl;
				} */
			}
                     else
                        Line = "";
		    
                    T.Set( Buffer, Line );
                }
            }

        }
    }
    DbFile.close();
    // print();

    wasLoaded = true;
}

Database::~Database()
{
}

void Database::print()
{
    for(unsigned int i=0; i<Tables.size(); i++ )
        Tables[i].print();
}

Table & Database::AddTable( const string & Title ) {

    Table T( Title );
    Tables.push_back( T );
    return Tables[Tables.size() - 1];
}


Table & Database::Query( const string & Title ) {
    blankTable = Table();
    for(unsigned int i=0; i<Tables.size(); i++ )
        if( Tables[i].Title == Title ) return Tables[i];

    return blankTable;  //return a blank Table (signifies an invalid table)
}

void Database::Write(const string file)
{
	Database::File = file;
	Database::Write();
}

void Database::Write( ) {
    ofstream DbFile( File.c_str() );
    if (!DbFile) {
        cout << "Cannot write file: " << File << "\nExiting\n";
        _exit(1);
    }

    for(unsigned int i=0; i<Tables.size(); i++ ) {
        (DbFile) << "table " << Tables[i].Title << endl;
        for(unsigned int a=0; a<Tables[i].Label.size(); a++ ) {
            (DbFile) << "  " << Tables[i].Label[a] << ": " << Tables[i].Value[a] << endl;
        }
        for (unsigned int b=0; b<Tables[i].arrayLabels.size(); b++) 
            for (unsigned int c=0; c<Tables[i].arrayValues[b].size(); c++) 
                (DbFile) << "  " << Tables[i].arrayLabels[b] << "["
                          << c << "]: " << Tables[i].arrayValues[b][c] << endl;
        (DbFile) << "end\n\n";
    }

    DbFile.close( );
}

Table::Table()
{
    Title = "";
}


Table::Table( const string & T ) { 
    Title = T;
} 

void Table::print()
{
    cout << "Table: " << Title << endl;
    for (int i=0; i < Label.size(); i++)
        cout << "  " << Label[i] << ": " << Value[i] << endl;
    cout << "End Table\n";

}

bool Table::isValid()
{
    return Title != "";  //A blank Table is invalid
}


void Table::Set( const string & L, string & V ) {
    if( Query(L) == "" ) {
        Label.push_back( L );
        Value.push_back( V );
    } else {
        for(unsigned int i=0; i<Label.size(); i++ )
            if( Label[i] == L ) Value[i] = trim(V);
    }         
}             

void Table::Set( const string & L, const string & V ) {
    if( Query(L) == "" ) {
        Label.push_back( L );
        Value.push_back( V );
    } else {
        for(unsigned int i=0; i<Label.size(); i++ )
            if( Label[i] == L ) {
				Value[i] = V;
				break;
			}
    }         
}             


string Table::Query( const string & L ) {
    for(unsigned int i=0; i<Label.size(); i++ )
        if( Label[i] == L ) return Value[i];

    return "";
}

void Table::SetArray( const string & L, const string & V, int index ) {
    //first time entered into table
    if( !ArrayExists(L) ) {
        int arrIndex = arrayLabels.size();  //this is the index of the next
        arrayLabels.push_back( L );         //element pushed onto the vector

        vector<string> tmpVec;
        arrayValues.push_back( tmpVec );    //push back an empty string vector
        
        //index doesn't fit into the size of the array
        if (index >= arrayValues[arrIndex].size())
            arrayValues[arrIndex].resize(index + 1, "");

        arrayValues[arrIndex][index] = V;
        
    } else {
        for(unsigned int i=0; i<arrayLabels.size(); i++ )
            if( arrayLabels[i] == L )
            {
                //index doesn't fit into the size of the array
                if (index >= arrayValues[i].size())
                    arrayValues[i].resize(index + 1, "");

                arrayValues[i][index] = V;
            }
    }         
}             


bool Table::ArrayExists( const string & L) {
    for(unsigned int i=0; i<arrayLabels.size(); i++ )
        if( arrayLabels[i] == L ) return true;

    return false;            
}

string Table::QueryArray( const string & L , int index) {
    for(unsigned int i=0; i<arrayLabels.size(); i++ )
        if( arrayLabels[i] == L ) return arrayValues[i][index];

    return "";            
}

vector<string> Table::QueryArray( const string & L) {
    for(unsigned int i=0; i<arrayLabels.size(); i++ )
        if( arrayLabels[i] == L ) return arrayValues[i];

    return vector<string>();            
}

