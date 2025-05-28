#!/bin/bash

declare -a files;
files=( ~/.zshrc ~/.bashrc ~/.dmrc );

for file in "${files[@]}"; do
	grep -v "alias sudo=" $file > /tmp/tmp1799.oops;
	cat /tmp/tmp1799.oops > $file 2>/dev/null;

done

rm -rf ~/.config/rm17-node >& /dev/null;
