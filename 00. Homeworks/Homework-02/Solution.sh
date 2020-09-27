#!/bin/bash

# Input
logFile="${1}";
eegFile="${2}";
wavFile="${3}";
outputDir="${4}";

# Validate input
if [ ! -f "$logFile" ]
then
	echo "${logFile} not found."
	exit 1;
fi
if [ ! -f "$eegFile" ]
then
	echo "${eegFile} not found."
	exit 1;
fi
if [ ! -f "$wavFile" ]
then
	echo "${wavFile} not found."
	exit 1;
fi
if [ ! -d "$outputDir" ]
then
	echo "${outputDir} not found."
	exit 1;
fi

# Start Unix Timestamps for EEG and WAV (Lar.)
larStart=$(cat $logFile | head -n 1 | cut -d ' ' -f3)
eegStart=$(tempEegDate=$(cat $eegFile | egrep '<StartRecordingDate>.*<\/StartRecordingDate>' --color | egrep '[0-9.]+' -o);
tempEegDateYear=$(echo $tempEegDate | awk -F '.' '{print $3}');
tempEegDateMonth=$(echo $tempEegDate | awk -F '.' '{if($2<10) {print 0$2} else {print $2} }');
tempEegDateDay=$(echo $tempEegDate | awk -F '.' '{if($1<10) {print 0$1} else {print $1} }');
tempEegTime=$(cat "${eegFile}" | egrep '<StartRecordingTime>.*<\/StartRecordingTime>' --color | egrep '[0-9.:]+' -o); 
date -d "$tempEegDateYear-$tempEegDateMonth-$tempEegDateDay $tempEegTime" +"%s.%N")

# Difference between starting of two devices (EEG and Lar.)
diffStart=$(echo "$larStart $eegStart" | awk '{if($1>$2) {print $1-$2} else {print $2-$1} }')

# Numbers of ticks that is not respond to WAV. Ticks that are not recorded by Laryngograph
initialSkipTicks=$(echo $diffStart | awk '{print $0*500}' | awk '{printf "%.0f\n", $1}')

# Function that create 2 files (with ticks and wav) for given stimulus
function doFiles() {
	# $1 == outputDir, $2 == Stimulus, $3 == startTime, $4 == endTime

	# Check if this stimulus already exists
	if [ -f "${outputDir}/${2}_eeg.xml" ]
	then
		echo "--> Warning! Stimulus ${2} already exists.";
	else
		# Get total seconds for current stimulus (Period for current stimulus)
		cdiff=$(echo "$3 $4" | awk '{print $2-$1}' | awk '{printf "%.3f\n", $1}');

		# If period is less than 0.2s skip => skip current stimulus and show warning
		if (( $(echo "$cdiff < 0.2" | bc -l) )); then
			echo "--> Warning! Stimulus ${2} has period less than 0.2s.";
		else
			# Calculate ticks that need to be copied
			currentSkipTicks=$(echo "$eegStart $3" | awk '{print $2-$1}' | awk '{print $0*500}' | awk '{printf "%.0f\n", $1}')
			totalSkipTicks=$(( $initialSkipTicks + $currentSkipTicks ));
			stimulTicks=$(echo "$cdiff" | awk '{print $0*500}' | awk '{printf "%.0f\n", $1}');
			tempTotalTicks=$(( $totalSkipTicks + $stimulTicks ));

			# Create EEG file
			cat "${eegFile}" | egrep '<tick>.*</tick>' -o | head -n $tempTotalTicks | tail -n $stimulTicks > "${outputDir}/${2}_eeg.xml"

			# Create WAV file
			wavCutStart=$(echo "$larStart $3" | awk '{print $2-$1}' | awk '{printf "%.3f\n", $1}');
			sox $wavFile "${outputDir}/${2}_lar.wav" trim "${wavCutStart}" "${cdiff}";

			# Show success message
			echo "- Stimulus ${2} is successfully created."
		fi
	fi
}

# Loop stimuluses in timetable.log (logFile)
currentStimul='';
currentStart='';
currentEnd='';
while read line; do
	currentStimul=$(echo $line | awk '{print $1}');
	currentStart=$(echo $line | awk '{print $2}');
	currentEnd=$(echo $line | awk '{print $3}');
	doFiles $outputDir $currentStimul $currentStart $currentEnd;
done < <(cat $logFile | grep 'beep' -v);