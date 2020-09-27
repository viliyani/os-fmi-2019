#!/bin/bash

if [ $(id -u) -ne 0 ]; then
	echo "You are not root!";
	exit 1;
fi

cat /etc/passwd | while read LINE; do
	user=$(echo "${LINE}" | cut -d ':' -f 1);
	uid=$(echo "${LINE}" | cut -d ':' -f 3);
	gid=$(echo "${LINE}" | cut -d ':' -f 4);
	home=$(echo "${LINE}" | cut -d ':' -f 6);

	if [ -z "${home}" ]; then
		echo "${user} has not homedir set";
	fi

	if [ ! -d "${home}" ]; then
		echo "${user} has invalid homedir";
	fi

	dirperm=$(ls -ld $home | awk '{print $1}');
	dirowner=$(ls -ld $home | awk '{print $1}');

	if [ "${dirowner}" != "${user}" ]; then
		echo "${user} is not owner of ${home}";
		continue;
	fi

	if [ $(echo $dirperm | cut -c 3) != "w" ]; then
		echo "${user} (owner) can't write in $home";
	fi
done