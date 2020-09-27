#!/bin/bash

if [ $# -ne 1 ]; then
	echo "No argument";
	exit 1;
fi

data="$(dirname ${1})/$(basename ${1})"
fpat="????-??-??-??-??-??.txt"

for buddy in $(find "$data" -mindepth 3 -maxdepth 3 -type d -exec basename {} \; | sort | uniq); do
	echo "$(cat ${data}/*/*/${buddy}/${fpat} | wc -l) ${buddy}";
done | sort -rn | head -n 10