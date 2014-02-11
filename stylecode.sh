#!/bin/bash

ASTYLE_VERSION="Artistic Style Version 2.01"

if [[ "`astyle --version 2>&1`" != ${ASTYLE_VERSION}* ]]; then
    echo "You should use: ${ASTYLE_VERSION}";
    exit 1;
fi

style="--style=allman --indent=spaces=3 --indent-namespaces --lineend=linux --min-conditional-indent=0 --indent-col1-comments"
options="--options=none --pad-header --unpad-paren --suffix=none --convert-tabs --add-brackets"

astyle $style $options src/*.cpp src/*.h
