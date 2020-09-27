#!/bin/bash

if [[ ! "${1}" =~ ^[1-9][0-9]*$ ]]; then
	echo "Invalid number";
	exit 1;
fi

if [ $(id -u) -ne 0 ]; then
	echo "You are not root!";
	exit 1;
fi

users=$(ps -e -o user= | sort | uniq);

for U in ${users}; do
	TOTAL_PS=0
	TOTAL_RSS=0
	MAX_RSS_PID=0
	MAX_RSS=0
	while read CPID CRSS; do
		TOTAL_PS=$(( $TOTAL_PS + 1 ));
		TOTAL_RSS=$(( $TOTAL_RSS + $CRSS ));
		MAX_RSS_PID=$CPID;
		MAX_RSS=$CRSS;
		# echo "${CPID} ${CRSS}";
	done < <(ps -u "${U}" -o pid=,rss= | sort -t ' ' -k 2 -n)
	echo "${U}	${TOTAL_RSS}	max: ${MAX_RSS}";

	if [ $MAX_RSS -gt ${1} ]; then
		echo -e "\tProcess ${MAX_RSS_PID} with ${MAX_RSS} RSS will be terminated!";
		kill -s SIGTERM $MAX_RSS_PID;
		sleep 2;
		kill -s SIGKILL $MAX_RSS_PID;
	fi
done