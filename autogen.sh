#!/bin/bash

mkdir -p m4

# Remove M4 files automatically copied (in case autotools version changes)
rm -f m4/libtool.m4 m4/ltoptions.m4 m4/ltsugar.m4 m4/ltversion.m4 m4/lt~obsolete.m4

autoreconf -fi;
rm -Rf autom4te*.cache;
