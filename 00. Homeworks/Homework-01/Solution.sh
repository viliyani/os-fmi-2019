#!/bin/bash
echo -e "$(cat text.txt | tr A-Z a-z | sed -r 's/,//g' | sed -r 's/\"//g' | sed -r 's/\.//g' | sed 's/ /\n/g' | sort | uniq)\n$(cat dic.txt | xargs -I '{}' grep -w -i -o {} text.txt | tr A-Z a-z | sort | uniq)" | sort | uniq -c | awk '$1==1 {print $2}' | xargs -I 'my_var' bash -c 'echo -n "my_var:"; tre-agrep -B -sw my_var dic.txt | head -n 1'
