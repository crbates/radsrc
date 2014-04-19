#!/bin/sh
#
# Shell script that provides "double-click" functionality equivalent to
# MacOSX application.
#
# Doug Wright

#....move to directory in which this command resides
cd `dirname $0`  >/dev/null 2>&1

#....execute test job
eval $file bin/radsrc reference/input.txt

#....compare output to reference
diff output.lin reference/output.lin

echo
echo The only difference should be the date.
echo

