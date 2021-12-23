#!/bin/bash

set -e

mkdir -p /root/.config/idesktop
cat<<EOF > /root/.config/idesktop/ideskrc
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
  Background.Source: $HOME/.config/idesktop
  Background.File: /usr/share/pixmaps/debian-logo.png
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
EOF

cat<<EOF > /root/.config/idesktop/home.lnk
table Icon
  Caption: Terminal
  CaptionTip: Run xterm
  Command: xterm
  Icon: /usr/share/icons/hicolor/scalable/apps/xterm-color.svg
  Width: 128
  Height: 128
  X: 32
  Y: 32
end
EOF

cat<<EOF > /root/.config/idesktop/test1.lnk
table Icon
  Caption: Test 1
  CaptionTip: Test 1
  Command: echo "Test"
  Icon: /root/.vnc/passwd
  Width: 128
  Height: 128
  X: 512
  Y: 512
end
EOF

cat<<EOF > /root/.config/idesktop/test2.lnk
table Icon
  Caption: Test 2
  CaptionTip: Test 2
  Command: echo "Test"
  Icon: /non-existing-file.png
  Width: 128
  Height: 128
  X: 256
  Y: 256
end
EOF

cat<<EOF > /root/.config/idesktop/test3.lnk
table Icon
  Caption: Test 3
  CaptionTip: Test 3
  Command: echo "Test"
  Icon: /bogus.png
  Width: 128
  Height: 128
  X: 256
  Y: 256
end
EOF

dd if=/dev/urandom of=/bogus.png bs=512 count=1

/usr/bin/Xvfb $DISPLAY -screen 0 1920x1080x24+32 &

## set x11vnc password to idesk
echo -e "idesk\nidesk\n" | x11vnc -storepasswd

## will only work with the mounted volume
src/idesk &

exec x11vnc -rfbauth /root/.vnc/passwd -display $DISPLAY -forever -autoport 5900 2>/dev/null
