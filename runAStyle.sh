#!/bin/bash
#echo "Copyright (c) 2019 J. W. Bruce '(AUTHORS)'"
#echo "All rights reserved."
#echo "(J. W. Bruce, jwbruce_AT_tntech.edu, Tennessee Tech University)"
#echo " "
#echo "    Permission to use, copy, modify, and distribute this software and its"
#echo "    documentation for any purpose, without fee, and without written agreement is"
#echo "    hereby granted, provided that the above copyright notice, the following"
#echo "    two paragraphs and the authors appear in all copies of this software."
#echo " "
#echo "    IN NO EVENT SHALL THE 'AUTHORS' BE LIABLE TO ANY PARTY FOR"
#echo "    OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE 'AUTHORS'"
#echo "    HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
#echo " "
#echo "    THE 'AUTHORS' SPECIFICALLY DISCLAIMS ANY WARRANTIES,"
#echo "    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY"
#echo "    AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS"
#echo "    ON AN 'AS IS' BASIS, AND THE 'AUTHORS' HAS NO OBLIGATION TO"
#echo "    PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
#echo " "
#echo "    Please maintain this header in its entirety when copying/modifying"
#echo "    these files."
#echo " "

# *************************************************
# runAStyle.sh - enforce code formatting standards
# *************************************************
# To maintain a consistent style, this batch file runs the `Artistic Style <http://astyle.sourceforge.net/>`_ code formatter on all sources in the library.
#
astyle --style=google --indent-switches --indent=spaces=2 --recursive --exclude=Drivers --exclude=build --lineend=linux --suffix=none --formatted --convert-tabs *.c,*.h
#
# Notes on options (see http://astyle.sourceforge.net/astyle.html for more information):
#
# --style=google          Attach brackets to their pre-block statements (e.g. Java )
# --indent-switches, -S   Indent ``switch`` blocks so that the ``case X:`` statements are indented in the switch block. The entire case block is indented.
# --indent=spaces, -s<#>  Indent using # spaces per indent
# --recursive, -r, -R     For each directory in the command line, process all subdirectories recursively.
# --suffix=none, -n       Do not retain a backup of the original file. The original file is purged after it is formatted.
# --lineend=linux         Force use of the specified line end style. Also -z2.
# --formatted, -Q         Formatted files display  mode. Display only the files that have been formatted. Do not display files that are unchanged.
# --convert-tabs, -c      Converts tabs into spaces in the non-indentation part of the line.

