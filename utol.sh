#!/bin/sh
# read dir input
echo "working directory: "
read dir

if [ $dir ]; then
	#move to given dir
	cd $dir
	#if cd fails -> exit0
	if [ $? ne 0 ]; then
		echo "error message! cannot access"
		exit 0
	fi
fi

for i in *; do
	if [ -f $i ]; then
		mv $i `echo $i | tr '[:upper:][:lower:]' '[:lower:][:upper:]'`
	fi
done
