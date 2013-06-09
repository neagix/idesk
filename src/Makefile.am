# src/Makefile.am for Idesk 0.7.2
# Copyright (c) 2005 FixXxeR (avelar at gmail.com)
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in 
# all copies or substantial portions of the Software. 
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL 
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.

#INCLUDES=-I$(top_srcdir) -DDEFAULT_PREFIX=\"@DEFAULT_PREFIX@\"
INCLUDES=-I$(top_srcdir)

BUILT_SOURCES = defaults.h
DISTCLEANFILES = defaults.h
CONFIG_CLEAN_FILES = defaults.h
defaults.h: Makefile
	echo '// This file is generated from makefile. Do not edit!' > defaults.h
	echo '#define DEFAULT_PREFIX "@DEFAULT_PREFIX@"' >> defaults.h
	echo '#define PACKAGE "@PACKAGE@"' >> defaults.h

idesk.$(OBJEXT): defaults.h

bin_PROGRAMS= idesk

idesk_SOURCES =  XDesktopContainer.h  XDesktopContainer.cpp \
		 XIcon.h XIcon.cpp \
                 XImlib2Image.h XImlib2Image.cpp \
                 Database.h Database.cpp \
		 DesktopConfig.h DesktopConfig.cpp \
 		 DesktopIconConfig.h DesktopIconConfig.cpp \
		 Misc.h Misc.cpp \
		 Util.h \
		 XImlib2Shadow.h XImlib2Shadow.cpp \
		 XIconWithShadow.h XIconWithShadow.cpp \
                 XImlib2Background.h XImlib2Background.cpp \
                 XImlib2ToolTip.h XImlib2ToolTip.cpp \
		 AbstractClasses.h \
 		 Action.h Action.cpp \
		 ActionConfig.h ActionConfig.cpp \
		 App.h App.cpp \
		 XImlib2Caption.h XImlib2Caption.cpp \
		 Timer.h Timer.cpp \
		 Main.h Main.cpp \
		 cursors.h

uninstall-local:
	for i in $(bin_PROGRAMS); do \
		rm -f $(exec_prefix)/bin/$$i; \
	done

