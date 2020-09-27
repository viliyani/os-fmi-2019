#!/bin/bash

if [ ! -d "${1}" ]; then
	echo "Invalid directory!";
	exit 1;
fi

case $# in 
	1)
		find -L "${1}" -type l 2>/dev/null
		;;
	2)
		for i in $(find "${1}" -type f 2>/dev/null); do
			if [ $(stat "${i}" --printf='%h') -gt $2 ]; then
				echo "${i}";
			fi
		done;
		;;
	*)
		echo "Give directory and number as arguments!";
		echo 1;
esac