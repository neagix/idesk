/* vim:tabstop=4:expandtab:shiftwidth=4
 * 
 * Idesk -- DesktopIconConfig.cpp
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

#include "DesktopIconConfig.h"

DesktopIconConfig::DesktopIconConfig(const string & fName, CommonOptions * parentData)
{
    iconFilename = fName;
    
    common = new CommonOptions();
    
    common->setDefaultsFromParent(*parentData);

    Database db(iconFilename);
    Table table = db.Query("Icon");

    if(!table.isValid())
        cout << "No Icon table found in the config file!!\n";
    else
    { 
        common->setOptions(table); 
        setIconOptions(table);
    }
}

DesktopIconConfig::~DesktopIconConfig()
{
}

void DesktopIconConfig::setIconOptions(Table table)
{
    pictureFilename = table.Query("Icon");
    picExtension = getExtension(pictureFilename);
    caption = table.Query("Caption");
    captionTip = table.Query("ToolTip.Caption");
    x = atoi(table.Query("X").c_str());
    y = atoi(table.Query("Y").c_str());
    width = atoi(table.Query("Width").c_str());
    height = atoi(table.Query("Height").c_str());

    if (table.ArrayExists("Command"))
        commandArray = table.QueryArray("Command");
    else if (table.Query("Command") != "")  //for a single command (non-array)
        commandArray.push_back(table.Query("Command"));
}

void DesktopIconConfig::saveIcon(int xCord, int yCord)
{
    x = xCord;
    y = yCord;

    Database db(iconFilename);
    Table & table = db.Query("Icon");
    
    if(table.isValid())
    {
        table.Set("X", itos(xCord));
        table.Set("Y", itos(yCord));
        
        db.Write();
    }

}

string DesktopIconConfig::getExtension(const string & file)
{
    string returnString;
    int pos = file.rfind('.', file.size() - 1); //search for '.' from the end

    if (pos == string::npos      || //no '.' is string
        pos == (file.size() - 1) || // '.' is at end of string
        pos == file.size() )        // '.' is at beginning of string 
    {
        cout << "Cannot determine file extension of: " << file << endl;
        returnString = "?";
    }
    else  //grab all of the string that is after the '.'
        returnString = getUpper(file.substr(pos + 1, file.size() - pos));
         
    if(returnString.size() > 3){  //remove all white space
	    while(returnString.size() > 3){
	       returnString =   returnString.substr(0, returnString.size() - 1); 
	    }	    
    }

    return returnString;
}

/***************************************************************************\
 * CommonOptions class                                                     *
\***************************************************************************/ 

CommonOptions::CommonOptions()
{
}

CommonOptions::~CommonOptions()
{
}

void CommonOptions::setCommonDefaults()
{
	
    fontName = "Arial";
    fontSize = 16;
    fontColor = "#ffffff";

    clickDelay = 200;

    shadowColor = "#000000";
    shadowX = 1;
    shadowY = 1;
    
    isBold = false;

    shadowOn = false;
    snapShadow = false;
    snapShadowTrans = -1;

    captionOnHover = false;
    captionPlacement = "Bottom";
    
    fillStyle = "None";
    
    cursorOver = XC_hand1;
}

void CommonOptions::setDefaultsFromParent(CommonOptions & other)
{
    fontName = other.fontName;
    fontSize = other.fontSize;
    fontColor = other.fontColor;
    
    clickDelay = other.clickDelay;

    shadowColor = other.shadowColor;
    shadowX = other.shadowX;
    shadowY = other.shadowY;
    
    isBold = other.isBold;

    shadowOn = other.shadowOn;
    snapShadow = other.snapShadow;
    snapShadowTrans = other.snapShadowTrans;

    captionOnHover = other.captionOnHover;

    captionPlacement = other.captionPlacement;
    
    cursorOver = other.cursorOver;
    
    fillStyle = other.fillStyle;
    
}

void CommonOptions::setOptions(Table t)
{
    table = t;

    //font options
    if (table.Query("FontName") != "")
        fontName = table.Query("FontName"); 
    if (table.Query("FontSize") != "")
        fontSize = atoi(table.Query("FontSize").c_str());
	if (fontSize>256)
	    fontSize = 16;
    if (table.Query("FontColor") != "")
        fontColor = table.Query("FontColor"); 

    //click options
    if (table.Query("ClickDelay") != "")
        clickDelay = atoi(table.Query("ClickDelay").c_str());

    //shadow fonts
    if (getUpper(table.Query("Shadow")) == "TRUE")
        shadowOn = true;
    else if (getUpper(table.Query("Shadow")) == "FALSE")
        shadowOn = false;

    if (table.Query("ShadowColor") != "")
        shadowColor = table.Query("ShadowColor");
    if (table.Query("ShadowX") != "")
        shadowX = atoi(table.Query("ShadowX").c_str());
    if (table.Query("ShadowY") != "")
        shadowY = atoi(table.Query("ShadowY").c_str());
    
    //boldness
    if (getUpper(table.Query( "Bold" )) == "TRUE")
        isBold = true;
    else if (getUpper(table.Query( "Bold" )) == "FALSE")
        isBold = false;

    //snap shadow
    if (getUpper(table.Query("SnapShadow")) == "TRUE")
        snapShadow = true;
    else if (getUpper(table.Query("SnapShadow")) == "FALSE")
        snapShadow = false;

    if (table.Query("SnapShadowTrans") != "")
        snapShadowTrans = atoi(table.Query("SnapShadowTrans").c_str());

    //captionOnHover
    if (getUpper(table.Query("CaptionOnHover")) == "TRUE")
        captionOnHover = true;
    else if (getUpper(table.Query("CaptionOnHover")) == "FALSE")
        captionOnHover = false;
       

    //captionPlacement
    if (getUpper(table.Query("CaptionPlacement")) == "BOTTOM")
        captionPlacement = "Bottom";
    else if (getUpper(table.Query("CaptionPlacement")) == "TOP")
        captionPlacement = "Top";
    else if (getUpper(table.Query("CaptionPlacement")) == "LEFT")
        captionPlacement = "Left";
    else if (getUpper(table.Query("CaptionPlacement")) == "RIGHT")
        captionPlacement = "Right";
    
   
    //fillStyle
    if (getUpper(table.Query("FillStyle")) == "FILLINVERT")
	    fillStyle = "FillInvert";
    else if (getUpper(table.Query("FillStyle")) == "FILLHLINE")
	    fillStyle = "FillHLine";
    else if (getUpper(table.Query("FillStyle")) == "FILLVLINE")
	    fillStyle = "FillVLine";
    else if (getUpper(table.Query("FillStyle")) == "None")
	    fillStyle = "None";

    //Cursor
    if (getUpper(table.Query("CursorOver")) == "X_CURSOR")
	    cursorOver = XC_X_cursor;
    else if (getUpper(table.Query("CursorOver")) == "ARROW")
	    cursorOver = XC_arrow;
    else if (getUpper(table.Query("CursorOver")) == "BASED_ARROW_DOWN")
	    cursorOver = XC_based_arrow_down;
    else if (getUpper(table.Query("CursorOver")) == "BASED_ARROW_UP")
	    cursorOver = XC_based_arrow_up;
    else if (getUpper(table.Query("CursorOver")) == "BOAT")
	    cursorOver = XC_boat;
    else if (getUpper(table.Query("CursorOver")) == "BOGOSiTY")
	    cursorOver = XC_bogosity;
    else if (getUpper(table.Query("CursorOver")) == "BOTTOM_LEFT_CORNER")
	    cursorOver = XC_bottom_left_corner;
    else if (getUpper(table.Query("CursorOver")) == "BOTTOM_RIGHT_CORNER")
	    cursorOver =  XC_bottom_right_corner;
    else if (getUpper(table.Query("CursorOver")) == "BOTTOM_SIDE")
	    cursorOver = XC_bottom_side;
    else if (getUpper(table.Query("CursorOver")) == "BOTTOM_TEE")
	    cursorOver = XC_bottom_tee;
    else if (getUpper(table.Query("CursorOver")) == "BOX_SPIRAL")
	    cursorOver =  XC_box_spiral;
    else if (getUpper(table.Query("CursorOver")) == "CENTER_PTR")
	    cursorOver = XC_center_ptr;
    else if (getUpper(table.Query("CursorOver")) == "CIRCLE")
	    cursorOver = XC_circle;
    else if (getUpper(table.Query("CursorOver")) == "CLOCK")
	    cursorOver = XC_clock;
    else if (getUpper(table.Query("CursorOver")) == "COFFE_MUG")
	    cursorOver = XC_coffee_mug;
    else if (getUpper(table.Query("CursorOver")) == "CROSS")
	    cursorOver = XC_cross;
    else if (getUpper(table.Query("CursorOver")) == "CROSS_REVERSE")
	    cursorOver = XC_cross_reverse;
    else if (getUpper(table.Query("CursorOver")) == "CROSSHAIR")
	    cursorOver = XC_crosshair;
    else if (getUpper(table.Query("CursorOver")) == "DIAMOND_CROSS")
	    cursorOver = XC_diamond_cross;
    else if (getUpper(table.Query("CursorOver")) == "DOT")
	    cursorOver = XC_dot;
    else if (getUpper(table.Query("CursorOver")) == "DOTBOX")
	    cursorOver = XC_dotbox;
    else if (getUpper(table.Query("CursorOver")) == "DOUBLE_ARROW")
	    cursorOver = XC_double_arrow;
    else if (getUpper(table.Query("CursorOver")) == "DRAFT_LARGE")
	    cursorOver = XC_draft_large;
    else if (getUpper(table.Query("CursorOver")) == "DRAFT_SMALL")
	    cursorOver = XC_draft_small;
    else if (getUpper(table.Query("CursorOver")) == "DRAPED_BOX")
	    cursorOver = XC_draped_box;
    else if (getUpper(table.Query("CursorOver")) == "EXCHANGE")
	    cursorOver = XC_exchange;
    else if (getUpper(table.Query("CursorOver")) == "FLEUR")
	    cursorOver =  XC_fleur;
    else if (getUpper(table.Query("CursorOver")) == "GOBBLER")
	    cursorOver = XC_gobbler;
    else if (getUpper(table.Query("CursorOver")) == "GUMBY")
	    cursorOver = XC_gumby;
    else if (getUpper(table.Query("CursorOver")) == "HAND1")
	    cursorOver = XC_hand1;
    else if (getUpper(table.Query("CursorOver")) == "HAND2")
	    cursorOver = XC_hand2;
    else if (getUpper(table.Query("CursorOver")) == "HEART")
	    cursorOver = XC_heart;
    else if (getUpper(table.Query("CursorOver")) == "ICON")
	    cursorOver = XC_icon;
    else if (getUpper(table.Query("CursorOver")) == "IRON_CROSS")
	    cursorOver = XC_iron_cross;
    else if (getUpper(table.Query("CursorOver")) == "LEFT_PTR")
	    cursorOver = XC_left_ptr;
    else if (getUpper(table.Query("CursorOver")) == "LEFT_PTR")
	    cursorOver = XC_left_side;
    else if (getUpper(table.Query("CursorOver")) == "LEFT_TEE")
	    cursorOver = XC_left_tee;
    else if (getUpper(table.Query("CursorOver")) == "LEFFBUTTON")
	    cursorOver = XC_leftbutton;
    else if (getUpper(table.Query("CursorOver")) == "LL_ANGLE")
	    cursorOver = XC_ll_angle;
    else if (getUpper(table.Query("CursorOver")) == "LR_ANGLE")
	    cursorOver = XC_lr_angle;
    else if (getUpper(table.Query("CursorOver")) == "MAN")
	    cursorOver = XC_man;
    else if (getUpper(table.Query("CursorOver")) == "MIDDLEBUTTON")
	    cursorOver = XC_middlebutton;
    else if (getUpper(table.Query("CursorOver")) == "MOUSE")
	    cursorOver = XC_mouse;
    else if (getUpper(table.Query("CursorOver")) == "PENCIL")
	    cursorOver = XC_pencil;
    else if (getUpper(table.Query("CursorOver")) == "PIRATE")
	    cursorOver = XC_pirate;
    else if (getUpper(table.Query("CursorOver")) == "PLUS")
	    cursorOver = XC_plus;
    else if (getUpper(table.Query("CursorOver")) == "QUESTION_ARROW")
	    cursorOver = XC_question_arrow;
    else if (getUpper(table.Query("CursorOver")) == "RIGHT_PTR")
	    cursorOver = XC_right_ptr;
    else if (getUpper(table.Query("CursorOver")) == "RIGHT_SIDE")
	    cursorOver = XC_right_side;
    else if (getUpper(table.Query("CursorOver")) == "RIGHT_TEE")
	    cursorOver = XC_right_tee;
    else if (getUpper(table.Query("CursorOver")) == "RIGHTBUTTON")
	    cursorOver = XC_rightbutton;
    else if (getUpper(table.Query("CursorOver")) == "RTL_LOGO")
	    cursorOver = XC_rtl_logo;
    else if (getUpper(table.Query("CursorOver")) == "SAILBOAT")
	    cursorOver = XC_sailboat;
    else if (getUpper(table.Query("CursorOver")) == "SB_DOWN_ARROW")
	    cursorOver = XC_sb_down_arrow;
    else if (getUpper(table.Query("CursorOver")) == "SB_H_DOUBLE_ARROW")
	    cursorOver = XC_sb_h_double_arrow;
    else if (getUpper(table.Query("CursorOver")) == "SB_LEFT_ARROW")
	    cursorOver = XC_sb_left_arrow;
    else if (getUpper(table.Query("CursorOver")) == "SB_RIGHT_ARROW")
	    cursorOver = XC_sb_right_arrow;
    else if (getUpper(table.Query("CursorOver")) == "SB_UP_ARROW")
	    cursorOver = XC_sb_up_arrow;
    else if (getUpper(table.Query("CursorOver")) == "SB_V_DOUBLE_ARROW")
	    cursorOver = XC_sb_v_double_arrow;
    else if (getUpper(table.Query("CursorOver")) == "SHUTTLE")
	    cursorOver = XC_shuttle;
    else if (getUpper(table.Query("CursorOver")) == "SIZING")
	    cursorOver = XC_sizing;
    else if (getUpper(table.Query("CursorOver")) == "SPIDER")
	    cursorOver = XC_spider;
    else if (getUpper(table.Query("CursorOver")) == "SPRAYCAN")
	    cursorOver = XC_spraycan;
    else if (getUpper(table.Query("CursorOver")) == "STAR")
	    cursorOver = XC_star;
    else if (getUpper(table.Query("CursorOver")) == "TARGET")
	    cursorOver = XC_target;
    else if (getUpper(table.Query("CursorOver")) == "TCROSS")
	    cursorOver = XC_tcross;
    else if (getUpper(table.Query("CursorOver")) == "TOP_LEFT_ARROW")
	    cursorOver = XC_top_left_arrow;
    else if (getUpper(table.Query("CursorOver")) == "TOP_LEFT_CORNER")
	    cursorOver = XC_top_left_corner;
    else if (getUpper(table.Query("CursorOver")) == "TOP_RIGHT_CORNER")
	    cursorOver = XC_top_right_corner;
    else if (getUpper(table.Query("CursorOver")) == "TOP_SIDE")
	    cursorOver = XC_top_side;
    else if (getUpper(table.Query("CursorOver")) == "TOP_TEE")
	    cursorOver = XC_top_tee;
    else if (getUpper(table.Query("CursorOver")) == "TREK")
	    cursorOver = XC_trek;
    else if (getUpper(table.Query("CursorOver")) == "UL_ANGLE")
	    cursorOver = XC_ul_angle;
    else if (getUpper(table.Query("CursorOver")) == "UMBRELLA")
	    cursorOver = XC_umbrella;
    else if (getUpper(table.Query("CursorOver")) == "UR_ANGLE")
	    cursorOver = XC_ur_angle;
    else if (getUpper(table.Query("CursorOver")) == "WATCH")
	    cursorOver = XC_watch;
    else if (getUpper(table.Query("CursorOver")) == "XTERM")
	    cursorOver = XC_xterm;
}
