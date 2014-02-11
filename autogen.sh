#! /bin/sh

touch NEWS README AUTHORS ChangeLog
aclocal
autoconf -f
automake --add-missing -c

