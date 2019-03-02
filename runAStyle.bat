REM .. "Copyright (c) 2019 J. W. Bruce ("AUTHOR(S)")"
REM    All rights reserved.
REM    (J. W. Bruce, jwbruce_AT_tntech.edu, Tennessee Tech University)
REM
REM     Permission to use, copy, modify, and distribute this software and its
REM     documentation for any purpose, without fee, and without written agreement is
REM     hereby granted, provided that the above copyright notice, the following
REM     two paragraphs and the authors appear in all copies of this software.
REM
REM     IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
REM     DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
REM     OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
REM     HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
REM
REM     THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
REM     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
REM     AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
REM     ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
REM     PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
REM
REM     Please maintain this header in its entirety when copying/modifying
REM     these files.
REM
REM *************************************************
REM runAStyle.bat - enforce code formatting standards
REM *************************************************
REM To maintain a consistent style, this batch file runs the `Artistic Style <http://astyle.sourceforge.net/>`_ code formatter on all sources in the library.
bin\astyle --style=google --indent-switches --indent=spaces=2 --recursive --exclude=build --lineend=linux --suffix=none --formatted --convert-tabs *.c *.h
@echo off
REM Notes on options (see http://astyle.sourceforge.net/astyle.html for more information):
REM
REM --style=google			Attach brackets to their pre-block statements (e.g. Java/K&R style).
REM --indent-switches, -S   Indent ``switch`` blocks so that the ``case X:`` statements are indented in the switch block. The entire case block is indented.
REM --indent=spaces, -s<#>  Indent using # spaces per indent
REM --recursive, -r, -R     For each directory in the command line, process all subdirectories recursively.
REM --suffix=none, -n       Do not retain a backup of the original file. The original file is purged after it is formatted.
REM --lineend=linux         Force use of the specified line end style. Also -z2.
REM --formatted, -Q         Formatted files display  mode. Display only the files that have been formatted. Do not display files that are unchanged.
REM --convert-tabs, -c      Converts tabs into spaces in the non-indentation part of the line.
:
