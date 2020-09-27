#!/bin/bash

# if [ $(id -u) -ne 0 ]; then
# 	echo "You are not root!";
# 	exit 1;
# fi

while read USERNAME HOMEDIR; do
	if [ -d "${HOMEDIR}" ]; then
		find "${HOMEDIR}" -type f -printf "${USERNAME} %T@ %p\n" 2>/dev/null
	fi
done < <(cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' '\t') | sort -t ' ' -k 1 -nr | head -n 1 | awk '{print $1 "\t" $3}'