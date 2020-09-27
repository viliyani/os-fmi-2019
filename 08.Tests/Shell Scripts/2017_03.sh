#!/bin/bash

if [ $(id -u) -ne 0 ]; then
	echo "You are not root!";
	exit 1;
fi

for U in $(ps -e -o user= | sort | uniq); do
	TOTAL_RSS=0;
	PS_COUNT=0;
	while read CPID CRSS; do
		PS_COUNT=$(( $PS_COUNT + 1 ));
		TOTAL_RSS=$(( $TOTAL_RSS + $CRSS ));
		MAX_RSS=${CRSS};
		MAX_RSS_PID=${CPID};
	done < <(ps -u "${U}" -o pid=,rss= | sort -n -k 2);

	if [ ${PS_COUNT} -gt 0 ]; then
		AVGRSS=$(( $TOTAL_RSS / $PS_COUNT ));
		echo "${U} ${PS_COUNT} ${TOTAL_RSS}";

		if [ ${MAX_RSS} -gt $(( ${AVGRSS} * 2 )) ]; then
			echo -e "\t${MAX_RSS_PID} has ${MAX_RSS} and will terminate";
			kill -s SIGTERM "${MAX_RSS_PID}";
			sleep 2;
			kill -s SIGKILL "${MAX_RSS_PID}";
		fi
	fi
done
