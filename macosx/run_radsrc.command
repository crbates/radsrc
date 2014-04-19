#!/bin/sh
#
# Shell script that provides "double-click" functionality equivalent to
# MacOSX application.
#
# Doug Wright

#....move to directory in which this command resides
cd `dirname $0`  >/dev/null 2>&1

#....prompt for possible input file
echo
echo -n "Enter input file name [or hit return for interactive mode] "
read file
echo

#....if the user specified a file, cat it to the program
#
#    I use a pipe instead of <file, because the user can
#    choose to enter 'filename -', this will cat the file
#    and let the user continue to type via standard input.
#    Note that if the user does use this feature (-) an
#    extra carriage return is expected at the end of 
#    
#[ "$file" ] && file="cat $file |"

#....execute actual binary
#eval $file bin/radsrc
eval bin/radsrc $file

