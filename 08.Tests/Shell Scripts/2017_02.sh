#!/bin/bash

check_dir() {
	if [ ! -d "${1}" ]; then
		echo "${1} is not a directory!";
		exit 1;
	fi
}

if [ $# -ne 3 ]; then
	echo "Give 3 arguments!";
	exit 1;
fi

if [ $(id -u) -ne 0 ]; then
	echo "You're not a root!";
	exit 1;
fi

check_dir "${1}"; 
check_dir "${2}";

if [ $(find "${2}" -type f 2>/dev/null | wc -l) -ne 0 ]; then
	echo "Destination directory is not empty!";
	exit 1;
fi

SRC=$(dirname "${1}")"/"$(basename "${1}");
DST=$(dirname "${2}")"/"$(basename "${2}");
ABC="${3}";

for item in $(find "${SRC}" -type f -name "*${ABC}*"); do
	DSTFILE=$(echo "${item}" | sed -e "s|${SRC}|${DST}|g");
	mkdir -p $(dirname "${DSTFILE}");
	mv "${item}" "${DSTFILE}";
done;