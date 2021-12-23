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

#include "DesktopConfig.h"
#include "Util.h"
#include <sys/stat.h>

// the initializer list just sets the program defaults for non-necessary options
DesktopConfig::DesktopConfig(Database db, string ideskrcFile) :
                             AbstractConfig(ideskrcFile)
{
    wasLoaded = db.wasLoaded;
    common = new CommonOptions();

    Table table = db.Query("Config");
    
    if (!table.isValid())
    {
        cerr << "Can't find config file or missing 'Config'"
             << " table in the config file.\n";
        _exit(1);
    }

    // handle the configure options
    setOptions(table);
    loadIcons();
}

DesktopConfig::~DesktopConfig()
{
    delete common;
}

void DesktopConfig::setOptions(Table table)
{
    setDefaults();

    //DesktopContainer only options
    setDesktopOnlyOptions(table);
    
    //Options that can be overridden in the icon config
    common->setCommonDefaults();
    common->setOptions(table);

    // TODO - add functionality for other protected members in DesktopConfig.h
}

void DesktopConfig::setDefaults()
{
    isLocked = false;
    snapOn = false;
    snapWidth = 1;
    snapHeight = 1;
    startSnapTop = startSnapLeft = true;
    
    fileBackground = "None";
    colorBackground = "None";
    sourceBackground = "None";
    delayBackground = 0; //Default Inactive
    modeBackground = "STRETCH";
    
    transparency = 0;
    
    fontNameTip = "Arial";
    fontSizeTip = 10;
    foreColorTip = "#000000";
    backColorTip = "#FFFACD";
    captionTipOnHover = false;
    captionTipPlacement = "Bottom";
}

void DesktopConfig::setDesktopOnlyOptions(Table table)
{
    string tmpStr;
    
    //locking
    if (getUpper(table.Query("Locked")) == "TRUE")
        isLocked = true;
    else if (getUpper(table.Query("Locked")) == "FALSE")
        isLocked = false;

    //snap options
    if (getUpper(table.Query("IconSnap")) == "TRUE")
        snapOn = true;
    else if (getUpper(table.Query("IconSnap")) == "FALSE")
        snapOn = false;

    snapWidth = atoi(table.Query("SnapWidth").c_str());
    snapHeight = atoi(table.Query("SnapHeight").c_str());

    tmpStr = getUpper(table.Query("SnapOrigin"));
    
    if (tmpStr == "BOTTOMRIGHT")
    {
        startSnapLeft = false;
        startSnapTop = false;
    }
    else if (tmpStr == "BOTTOMLEFT")
        startSnapTop = false;
    else if (tmpStr == "TOPRIGHT")
        startSnapLeft = false;
    // last case automatically handled with default of TOPLEFT
    
     //File Background
      if (table.Query("Background.File") != "")
	    fileBackground = table.Query("Background.File");
      
      if(fileBackground != "" && fileBackground != "None") {
	      struct stat b;
	      string directory(getenv("HOME"));
	      char * s = (char *)fileBackground.c_str();
	      directory = s[0]=='~' ? directory + &s[1]:s;
	      if( stat( directory.c_str(), &b ) < 0 ){
		      cerr << "[idesk] Background file \"" << fileBackground << "\" not found." << endl;
		      fileBackground = "None";
	      }else{
		      fileBackground = directory;  
	      }
      }
      
       //Color Background
      if (table.Query("Background.Color") != "")
	    colorBackground = table.Query("Background.Color");
      
      //Source Background
      if (table.Query("Background.Source") != "")
	      sourceBackground = table.Query("Background.Source");
      
      if(sourceBackground != "" && sourceBackground != "None"){
	      struct stat b;
	      string directory(getenv("HOME"));
	      char * s = (char *)sourceBackground.c_str();
	      directory = s[0]=='~' ? directory + &s[1]:s;
	      if( stat( directory.c_str(), &b ) < 0 ) {
		      sourceBackground ="None";
		      cerr << "[idesk] Background source not found." << endl;
	      }else{
		      sourceBackground = directory;  
	      }
      }
      
      //Delay Background
      if (table.Query("Background.Delay") != "")
	      delayBackground  = atoi(table.Query("Background.Delay").c_str());
      //Max 1 year
      if(delayBackground > 525600) 
	      delayBackground = 525600;
      //Min 0 minute with this value should inactive
      if(delayBackground < 0) 
	      delayBackground = 0;
      
      //Mode Background
      if (getUpper(table.Query("Background.Mode")) == "SCALE")
	      modeBackground = getUpper(table.Query("Background.Mode"));
      if (getUpper(table.Query("Background.Mode")) == "MIRROR")
	      modeBackground = getUpper(table.Query("Background.Mode")); 
      if (getUpper(table.Query("Background.Mode")) == "FIT")
	      modeBackground = getUpper(table.Query("Background.Mode"));   
      if (getUpper(table.Query("Background.Mode")) == "CENTER")
	      modeBackground = getUpper(table.Query("Background.Mode"));
      if (getUpper(table.Query("Background.Mode")) == "STRETCH")
	      modeBackground = getUpper(table.Query("Background.Mode"));   
      
      //transparency
      if (table.Query("Transparency") != "")
        transparency = atoi(table.Query("Transparency").c_str());
      
      //font options for tooltip
      if (table.Query("ToolTip.FontName") != "")
	    fontNameTip = table.Query("ToolTip.FontName"); 
      
      if (table.Query("ToolTip.FontSize") != "")
	    fontSizeTip = atoi(table.Query("ToolTip.FontSize").c_str());
      if (fontSizeTip>256)
              fontSizeTip = 16;
      
      if (table.Query("ToolTip.ForeColor") != "")
	    foreColorTip = table.Query("ToolTip.ForeColor");
      if (table.Query("ToolTip.BackColor") != "")
	    backColorTip = table.Query("ToolTip.BackColor");
      
      //captionTipOnHover
      if (getUpper(table.Query("ToolTip.CaptionOnHover")) == "TRUE")
        captionTipOnHover = true;
      else if (getUpper(table.Query("ToolTip.CaptionOnHover")) == "FALSE")
        captionTipOnHover = false;

     //captionTipPlacement
     if (getUpper(table.Query("ToolTip.CaptionPlacement")) == "BOTTOM")
	    captionTipPlacement = "Bottom";
     else if (getUpper(table.Query("ToolTip.CaptionPlacement")) == "TOP")
	    captionTipPlacement = "Top";
     else if (getUpper(table.Query("ToolTip.CaptionPlacement")) == "LEFT")
	    captionTipPlacement = "Left";
     else if (getUpper(table.Query("ToolTip.CaptionPlacement")) == "RIGHT")
	    captionTipPlacement = "Right";
     else if (getUpper(table.Query("ToolTip.CaptionPlacement")) == "AUTO")
            captionTipPlacement = "Auto";
}


void DesktopConfig::loadIcons()
{
    struct dirent **files;
    char * tmp;
    string xdgConfigHome;
    string homeDirectory;

    // see https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
    tmp = getenv("XDG_CONFIG_HOME");
    if (tmp) {
        xdgConfigHome.assign(tmp);
    }

    tmp = getenv("HOME");
    if (tmp) {
        homeDirectory.assign(tmp);
    }

    if (xdgConfigHome.empty()) {
        xdgConfigHome = homeDirectory + "/.config";
    }

    string idesktopDir = xdgConfigHome + "/idesktop/";

    int fileCount = scandir(idesktopDir.c_str(), &files, 0, alphasort);
    if (fileCount == -1)
    {
        cerr << "No icons found in " << idesktopDir << " - trying legacy location ~/.idesktop\n";
        idesktopDir = homeDirectory + "/.idesktop/";
        fileCount = scandir(idesktopDir.c_str(), &files, 0, alphasort);
        if (fileCount == -1) {
            cerr << "No icons found in " << idesktopDir << "\n";
            return;
        }
    }

    for(int i = 0; i < fileCount; i++)
    {
        if (!backgroundFile(files[i]->d_name))
        {
            string filename = idesktopDir + files[i]->d_name;
            
            if (filename.size() > 4 && filename.substr(filename.size()-4,filename.size()) == ".lnk")
            {
				Database db = Database(filename, false);
				Table table = db.Query("Icon");

				if (table.isValid())
				{   
					DesktopIconConfig *iconPtr = new DesktopIconConfig(filename, table, common); 
					iconConfigList.push_back(iconPtr);
				} else
					cerr << "Error: \"" << files[i]->d_name << "\" is not a valid .lnk desktop icon\n";
			} else if (filename.size() > 8 && filename.substr(filename.size()-8,filename.size()) == ".desktop")
			{
/*				FreeDesktopIcon fdi(filename);
				
				if (fdi.isValid())
				{
					iconPtr = new DesktopIconConfig(filename, fdi, common); 
					iconConfigList.push_back(iconPtr);
				} else */
					cerr << "Error: \"" << files[i]->d_name << "\" is not a valid .desktop desktop icon\n";
			} else
				cerr << "Warning: \"" << files[i]->d_name << "\" is not a recognized desktop icon (.lnk or .desktop)\n";

            free(files[i]);
        }
    }
    free(files);
}

void DesktopConfig::saveLockState(bool lockState)
{
    if (!wasLoaded) {
        return;
    }

    Database db = Database(ideskrcFile, true);
    Table & table = db.Query("Config");
    
    if (db.wasLoaded) {
        if(table.isValid())
        {
            if (lockState)
                table.Set("Locked", "true");
            else
                table.Set("Locked", "false");

            db.Write();
        }
        else
        {
            cerr << "Incorrect config file\n";
            return;
        }
    }
}

bool DesktopConfig::backgroundFile(const string & filename)
{
    bool returnBool = false;
    
    if (filename.size() > 0 && (
            filename[0] == '.' ||
	filename[filename.size() - 1] == '~' ))
        returnBool = true;
    else if (filename.size() > 4 && filename.substr(filename.size()-4,filename.size()) != ".lnk")
	    returnBool = true;

    return returnBool;
}

