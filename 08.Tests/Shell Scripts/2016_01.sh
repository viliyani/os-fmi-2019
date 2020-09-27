#!/bin/bash

if [ ! -d "${1}" ]; then
	echo "Invalid directory!";
	exit 1;
fi

find -L "${1}" -type l