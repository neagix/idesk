# Idesk User README

Idesk is program that draws desktop icons. Each icon will execute a shell command on a configurable action. The icons can be moved on the desktop by dragging them, and the icons will remember their positions on start-up. Oh yea, forgot there is cool eye-candy.

Supported icon formats:
* JPEG
* GIF
* PPM
* PGM
* XPM
* TIFF
* PNG
* SVG (if compiled with --enable-svg)

NOTE: there are some problems with librsvg's usage of some advanced SVG files: they _cannot_ be loaded by librsvg.

## Dependencies

See runtime dependencies in docker/Dockerfile.preview and build time dependencies in docker/Dockerfile.

## Build and installation

You can build `idesk` in a docker container by running:
```
make -C docker idesk
```

If you wish to build natively on your machine install the dependencies in `docker/Dockerfile` on your system and then run:
```
autoreconf --install
./configure --enable-svg --enable-libsn
make -j
```

Read the INSTALL file for more information.

## Preview

To preview idesk in a container run:
```
make -C docker preview
```
This will start a container with X11 and x11vnc and allow you access from your host system with `vncviewer :5900` and password `idesk`.

## Optional Features:
  Idesk supports the XShape extension of X11R6.  This support is enabled by
default, but may be overridden by specifying `--disable-shape' on the configure
script's command line.

  Idesk provides startup notification support. This change the default X cursor for a clock cursor (in wait form), when you click an icon and the application launch, the cursor change and wait for application finished for startup. You need the libraries for startup notification, you can download that from:
 
 http://www.freedesktop.org/software/startup-notification

It is disabled by default but can be overridden by specifying
'--enable-libsn' on the configure script's command line.  

  Idesk provides SVG files support via librsvg and gdk-pixbuf
It is disabled by default but can be overridden by specifying
'--enable-svg' on the configure script's command line.

## Config file setup

To use Idesk you must first setup your config file. Put something like this in `~/.config/idesktop/ideskrc`.

```
table Config
  FontName: helvetica
  FontSize: 12
  FontColor: #FFFFFA
  FontNameTip: helvetica
  FontSizeTip: 9
  ForeColorTip: #0000FF
  BackColorTip: #FFFFFF
  PaddingX: 10
  PaddingY: 10
  Transparency: 75
  HighContrast: true
  Locked: false
  Shadow: false
  ShadowColor: #0006FA
  ShadowX: 2
  ShadowY: 2
  SnapShadow: true
  IconSnap: true
  SnapWidth: 10
  SnapHeight: 10
  SnapOrigin: BottomRight
  SnapShadowTrans: 300
  CaptionOnHover: true
  Bold: true
  ToolTip.FontSize: 11
  ToolTip.FontName: gothic
  ToolTip.ForeColor: #0000FF
  ToolTip.BackColor: #FFFFFF
  ToolTip.CaptionOnHover: true
  ToolTip.CaptionPlacement: Right
  Background.Delay: 0
  Background.Source: /home/aavelar/.idesktop/icons
  Background.File: /usr/local/share/gfilerunner/themes/default/search.png
  Background.Mode: Mirror
  Background.Color: #C2CCFF
end

table Actions
  Lock: control right doubleClk
  Reload: middle doubleClk
  Drag: left hold
  EndDrag: left singleClk
  Execute[0]: left doubleClk
  Execute[1]: right doubleClk
end
```

- "FontName" is the name Font. Default is Arial.

- "FontSize" is the size Font. Default is 16.

- "ForeColor" is the Foreground color. Default is #000000 (black).

- "ToolTip.FontName" is the name Font for tooltip. Default is Arial.

- "ToolTip.FontSize" is the size Font for the tooltip. Default is 10.

- ToolTip.ForeColor" is the Foreground color for the tooltip window. Default is #FFFFFF (white).

- "ToolTip.BackColor" is the Background color for the tooltip window. Default is #FFFACD.

- "ToolTip.CaptionPlacement" defines the position for Image ToolTip Text. Valid values are: Top, Bottom, Left, Right.

- "ToolTip.CaptionOnHover" is a boolean (true/false) value. Setting it to true will make the icon's tip text appear when the mouse hovers over an icon. False is the default behavior.

- "Locked" is boolean (true/false). Setting it to true disables the dragging movement.

- "Transparency" is between 0 (no glow effect) and 255 (invisible without a mouseover).

- "HighContrast" will draw a border around the text in the opposite colour if it is true.  It will draw it in an arbitary colour if it "#aa00aa", for example.  And it will not draw anything at all if it is set to "false".

- "Shadow" is boolean (true/false). Turning this on will draw a drop shadow of color ShadowColor behind the text. The shadow will be ShadowX pixels right and ShadowY pixels down (negative values will work). Both ShadowX and ShadowY default to 1.

- "Bold" is a boolean (true/false). Setting it to true will make your font bold.

- "Click Delay" is the number of milliseconds that represents a double-click.

- "SingleClick" is a boolean (true/false). Setting it to true makes a single-click execute the command associated with the icon. This also means that it is impossible to drag the icon either.

- "IconSnap" is a boolean (true/false). Setting it to true makes the icons snap to a grid defined by "SnapWidth" and "SnapHeight". The icon will snap to the middle of the rectangle defined by "SnapWidth" and "SnapHeight". So if "SnapWidth" = "SnapHeight" = 100, the center of the icon will by in multiples of 50: (50,50), or (50, 100), or (100, 100), or (250, 350)... You will want to make "SnapWidth" and "SnapHeight" greater than or equal to the size of the picture.

- "SnapWidth" is a positive integer, see description in "IconSnap".
- "SnapHeight" is a positive integer, see description in "IconSnap".

- "SnapOrigin" defines where the snap grip starts from. Valid values are: 
  TopLeft, TopRight, BottomLeft, BottomRight.

- "SnapShadow" is a boolean (true/false) value. Setting it to true will make a shadow of the icon appear where the icon will snap if the drag was released there. Turn "IconSnap" on and check this out, pretty cool effect.  

- "SnapShadowTrans" is a integer between 0 (opaque) and 255 (invisible). It represents the transparency of the SnapShadow

- "CaptionPlacement" defines the position for Image Caption. Valid values are:
  Top, Bottom, Left, Right.

- "CaptionOnHover" is a boolean (true/false) value. Setting it to true will make the icon's caption only appear when the mouse hovers over an icon. False is the default behavior.

- "FillStyle" defines the icon appearance when the user gives click him. The icon back to normal form when the user release the mouse button. FillInvert is the default behavior.

- "Background.File" defines the background image for wallpaper. If the path is wrong or the image has a bad format then doesn't put nothing. The image is set to original size only in tile center style (for now). This image remains although  idesk program ins't running. (Default: None). 

- "Background.Delay" Set the image change time in minutes format.  (Default: 0 min). Set 0 if you want to leave inactive the changer background system. Max value is 525600 min (forever I think).

- "Background.Source" Let you can define the folder where there are some images. If in the folder there aren't images or the folder didn't exist, then idesk looking for Background.File option.  (Default: None).

- "Background.Mode" Is the display mode for the images, the possible values are: Stretch, Scale, Center, Fit o Mirror).  (Default: Stretch).

- "Background.Color" Just apply when you set Center or Fit options for Background.Mode, the color will be set on corners and borders (Default: None, will be use the current pixmap).

- "CursorOver" defines the mouse cursor type when the mouse hovers over an icon. Valid values are:

     num_glyphs
     X_cursor
     arrow
     based_arrow_down
     based_arrow_up
     boat
     bogosity
     bottom_left_corner
     bottom_right_corner
     bottom_side
     bottom_tee 
     box_spiral
     center_ptr
     circle
     clock
     coffee_mug
     cross
     cross_reverse
     crosshair 
     diamond_cross
     dot 
     dotbox 
     double_arrow
     draft_large
     draft_small
     draped_box
     exchange
     fleur
     gobbler
     gumby
     hand1 
     hand2 
     heart
     icon 
     iron_cross 
     left_ptr 
     left_side 
     left_tee
     leftbutton 
     ll_angle 
     lr_angle 
     man 
     middlebutton 
     mouse 
     pencil 
     pirate 
     plus 
     question_arrow 
     right_ptr 
     right_side 
     right_tee 
     rightbutton 
     rtl_logo 
     sailboat 
     sb_down_arrow 
     sb_h_double_arrow 
     sb_left_arrow 
     sb_right_arrow 
     sb_up_arrow 
     sb_v_double_arrow 
     shuttle 
     sizing 
     spider 
     spraycan 
     star 
     target 
     tcross 
     top_left_arrow 
     top_left_corner 
     top_right_corner 
     top_side 
     top_tee 
     trek 
     ul_angle 
     umbrella 
     ur_angle 
     watch 
     xterm 


## Better Snap Explanation


               .          .          .
               .          .          .
               |__________|__________|_______ . . .
               |          |          |
               |          |          |
           y   |          |          |
               |          |          |
               |          |          |
         [>    |__________|__________|________ . . .
         |           x
         |           
         |

This is the bottom left of the desktop (SnapOrigin: BottomLeft)

X represents the snap width
Y represents the snap height
Snap origin only describes which corner of the screen

So basically set snap width to the width of the "box" you want the icon contained in and set snap height to the height of the "box" you want the icon contained in.

The icon will snap to the middle of that box specified by snap width and snap height. You will typically want the width and heihgt to be a bit larger than the width and height of the icons (make the height bigger if you use a caption).

## Action Configuration

In the Action table 5 different actions can be configured.

    Lock:       Disables icon dragging to prevent accidental movement.
    Reload:     Reloads all of the config options and gets a new background.
    Drag:       Puts the icon in drag mode. Icon follows cursor.
    EndDrag:    End the drag mode, icon will be positioned at the cursor.
    Execute[i]: Executes the ith icon command. So the Execute[0] action is the
                default action and run Command[0] (in the icon config file),
                Execute[1] runs Command[1] and so forth.

The options for each command are as follows:
    
    <shift> <control> <alt> [Button] [ClickType]
    [Button] = left, middle, or right (more than one is allowed)
    [ClickType] = singleClk, doubleClk, tripleClk, or hold

Some examples:

    The default settings, should be self explanitory.
        Lock: right doubleClk
        Reload: middle doubleClk
        Drag: left hold  <== Drag starts when left button is pressed.
        EndDrag: left singleClk <== Drag ends when left button is released.
        Execute[0]: left doubleClk
        
    Other examples

        Left single click while holding shift on the keyboard
        Execute[0]: shift left singleClk

        Left double click while holding down the right mouse button.
        Execute[0]: left doubleClk right hold

        And for those with too many fingers, middle double click while holding
        down the left and right mouse buttons, and control, alt, and shift
        on the keyboard.
        Execute[0]: control shift alt left hold middle doubleClk right hold
        
    Notes:
        For the Drag action I reccomend only doing button holds and for the
        EndDrag I recommend using single clicks. This way the icon begins
        dragging as soon as you press the button and stops dragging as soon
        as you release the button. Using other actions should work as expected,
        but there are a few small bugs.

        If two actions have the same key/button combinations they both will
        occur, unless one is the reload action. The reload action always only
        happens by itself.
        

## Icon setup

To add an icon, you need to first create a `~/.config/idesktop` directory and inside that directory create a file with `.lnk` extension, for example `home.lnk`, with content:

```
table Icon
  Caption: Home
  CaptionTip: This is my home
  Command: nautilus /home/you
  Icon: /home/you/.idesktop/home.png
  Width: 600
  Height: 700
  X: 680
  Y: 174
end
```

These are the options for icon file, they should mostly be self-explantory. The Icon option is the graphic file for the icon image. The Execute[i] action (in `ideskrc`) runs the Command[i]. If the Caption field is blank or just one space, then no caption will be created for the icon.

```
table Icon
  Caption: Tux
  CaptionTip: Linux rocks
  Icon: /home/you/icons/tux2.svg
  Width: 600
  Height: 700
  X: 680
  Y: 174
  Command[0]: echo 'Linux rocks!'
end
```

## Default Usage

- Double click with the first mouse button will run the command.
- When you click the icon change its appearance (depend the fill style).
- Click and drag with the first mouse button moves the icon (if not locked).
- Double-click with the middle mouse button reloads the icons and regenerates
  background. Useful after changing wallpapers to clear graphical artifacts.
- Double-click with the right mouse button to toggle the lock state. The
  state will be saved in the config file, so it will be remembered next startup.
