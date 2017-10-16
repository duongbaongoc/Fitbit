/*****************************************************************************************************************
* Programmer: Ngoc Bao Duong																					 *
* Class: CptS 122.01                                                                                             *
* Programming Assignment: Assignment 1																			 *
* Date: 06/10/2017												                         						 *
* Description: This assignment analyzes data that was generated from a real Fitbit device.						 *
* Note: Assuming the csv data ends with "23:59:00,0.968900025,0,0,68,0,1" - no "\n"								 *
*****************************************************************************************************************/
#include "header.h"

int main(void)
{
	//variable 
	FILE *infile = NULL, *outfile = NULL;
	char line[100] = "";
	FitbitData FitbitDataArray[1440];
	unsigned int recordIndex = 0, heartRateCount = 0;
	double totalCalories = 0.0, totalDistance = 0.0;
	double aveHeartRate = 0.0;
	unsigned int totalFloors = 0, totalSteps = 0,
		totalHeartRate = 0;
	unsigned int maxSteps = 0, currentSumPoorSleep = 0,
		maxSumPoorSleep = 0;
	char timeMaxSteps[9] = "", currentTimeStartPoorSleep[9] = "",
		currentTimeEndPoorSleep[9] = "", maxTimeStartPoorSleep[9] = "Nope.",
		maxTimeEndPoorSleep[9] = "Nope.";

	//initialize the array
	iniArray(FitbitDataArray);

	//open files
	infile = fopen("FitbitData.csv", "r");
	outfile = fopen("Results.csv", "w");

	if (infile == NULL)
		puts("File can not open!");

	//if infile opens successfully, read all data then store in the array
	else
	{
		//read the first line to discard; actual data starts from the second line
		fgets(line, 100, infile);
		//read all data
		while (!feof(infile))
		{
			//read each record from the file
			fgets(line, 100, infile);
			//get minute of each record
			strcpy(FitbitDataArray[recordIndex].minute, strtok(line, ","));
			//get calories of each record
			strcpy(line, strtok(NULL, ""));
			FitbitDataArray[recordIndex].calories = getToken(line);
			FitbitDataArray[recordIndex].distance = getToken(line);
			FitbitDataArray[recordIndex].floors = (unsigned int) getToken(line);
			FitbitDataArray[recordIndex].heartRate = (unsigned int) getToken(line);
			FitbitDataArray[recordIndex].steps = (unsigned int) getToken(line);
			if (*line != ',') //if there is a sleepLevel, get the token
				FitbitDataArray[recordIndex].sleepLevel = (Sleep) atof(line);
			// Note: if there is no sleepLevel -> leave as 0
			recordIndex++;
		}
		fclose(infile);
	}
	
	for (recordIndex = 0; recordIndex < 1440; recordIndex++)
	{
		//compute the total calories burned, distance walked in miles, floors walked, and steps taken
		//is FitbitData's field is empty, field in array is stored as 1000
		if (FitbitDataArray[recordIndex].calories < 1000) 
			totalCalories += FitbitDataArray[recordIndex].calories;
		if (FitbitDataArray[recordIndex].distance < 1000)
			totalDistance += FitbitDataArray[recordIndex].distance;
		if (FitbitDataArray[recordIndex].floors < 1000)
			totalFloors += FitbitDataArray[recordIndex].floors;
		if (FitbitDataArray[recordIndex].steps < 1000)
			totalSteps += FitbitDataArray[recordIndex].steps;

		//compute total heartrate
		if (FitbitDataArray[recordIndex].heartRate < 1000)
		{
			totalHeartRate += FitbitDataArray[recordIndex].heartRate;
			heartRateCount++;
		}

		//find max steps taken in one minute
		if (FitbitDataArray[recordIndex].steps > maxSteps &&
			FitbitDataArray[recordIndex].steps < 1000)
		{
			maxSteps = FitbitDataArray[recordIndex].steps;
			strcpy(timeMaxSteps, FitbitDataArray[recordIndex].minute);
		}
	}

	//compute average heartrate over the 24 hour period
	aveHeartRate = (double) totalHeartRate / heartRateCount;
	
	//find longest consecutive range of poor sleep
	for (recordIndex = 0; recordIndex < 1440; recordIndex++)
	{
		//if a poor sleep range is found, calculate the range's sum
		if (FitbitDataArray[recordIndex].sleepLevel > 1 && 
			FitbitDataArray[recordIndex].sleepLevel< 1000)
		{
			currentSumPoorSleep = 0;
			strcpy(currentTimeStartPoorSleep, FitbitDataArray[recordIndex].minute);
			while (FitbitDataArray[recordIndex].sleepLevel > 1 && 
				FitbitDataArray[recordIndex].sleepLevel< 1000)
			{
				currentSumPoorSleep += FitbitDataArray[recordIndex].sleepLevel;
				recordIndex++;
			}
			strcpy(currentTimeEndPoorSleep, FitbitDataArray[recordIndex - 1].minute);
			//compare if this range has higher sum than the max sum
			if (currentSumPoorSleep > maxSumPoorSleep)
			{
				maxSumPoorSleep = currentSumPoorSleep;
				strcpy(maxTimeStartPoorSleep, currentTimeStartPoorSleep);
				strcpy(maxTimeEndPoorSleep, currentTimeEndPoorSleep);
			}
		}
	}
	
	//write results into the screen
	puts("Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep");
	printf("%.3lf, %.3lf, %d, %d, %.3lf, %d, %s:%s\n", totalCalories, totalDistance,
		totalFloors, totalSteps, aveHeartRate, maxSteps, maxTimeStartPoorSleep, maxTimeEndPoorSleep);

	//write results into outfile
	fputs("Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep\n", outfile);
	fprintf(outfile, "%.3lf, %.3lf, %d, %d, %.3lf, %d, %s:%s", totalCalories, totalDistance, 
		totalFloors, totalSteps, aveHeartRate, maxSteps, maxTimeStartPoorSleep, maxTimeEndPoorSleep);

	fclose(outfile);

	return 0;
}