/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- Util.h
 *
 * Copyright (c) 2005, FixXxeR (avelar@gmail.com)
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

#ifndef UTIL_CLASS
#define UTIL_CLASS

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class Util
{
    public:
	 Util();
	~Util();
	static void copy(string source, string target){
		string Buffer;
		cout << "Loaded default file: " << source <<"\n";
	
		ofstream ofs( target.c_str() );
		if (!ofs) {
			cout << "Cannot write file: " << target << "!! Exiting.\n";
			_exit(1);
		}
	
		ifstream ifs;
		ifs.open(source.c_str(),ios::in);
	
		if (!ifs.is_open()) {
			cout << "Cannot read file: " << source << "!! Exiting.\n";
			_exit(1);
		}
		while( ifs ) {
			getline( ifs, Buffer );
			(ofs) << Buffer << endl;
		}
		ifs.close();
		ofs.close();
	};	
};

#endif
