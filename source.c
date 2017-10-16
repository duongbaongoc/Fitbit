/*****************************************************************************************************************
* Programmer: Ngoc Bao Duong																					 *
* Class: CptS 122.01                                                                                             *
* Programming Assignment: Assignment 1																			 *
* Date: 06/10/2017												                         						 *
* Description: This assignment analyzes data that was generated from a real Fitbit device.						 *
* Note: Assuming the csv data ends with "23:59:00,0.968900025,0,0,68,0,1" - no "\n"								 *
*****************************************************************************************************************/
#include "header.h"

/********************************************************************************
* Function: iniArray()															*
* Date Created: 06/10/2017													    *
* Description: This function initializes an FitbitData array.				    *
* Input parameters: pointer to a FitbitData array								*
* Returns: updated values of the array										    *
* Preconditions: at beginning of program									    *
* Postconditions: Each field of each record in the array is set to 0 or NULL	*
********************************************************************************/
void iniArray(FitbitData *pArray)
{
	strcpy(pArray -> minute, "hh");
	pArray-> calories = 0.0;
	pArray-> distance = 0.0;
	pArray-> floors = 0;
	pArray-> heartRate = 0;
	pArray-> steps = 0;
	pArray-> sleepLevel = 0;
}

/********************************************************************************
* Function: getToken()															*
* Date Created: 06/10/2017													    *
* Description: This function read the first token of a line if there is a token,*
*				then cast it to double number.								    *
* Input parameters: pointer to a line (string)									*
* Returns: updated line with the first token remove, the first token (double)   *
* Preconditions: at beginning of program; the line must have at least 1 comma   *
* Postconditions: return 1000 if there is no first token (,,), return its double*
*					value if there is one; first token or the comma is removed	*
*					from the line.												*
********************************************************************************/
double getToken(char *line)
{
	double result = 1000;
	if (*line != ',') //if there is an entry, get the token
	{
		result = atof(strtok(line, ","));
		strcpy(line, strtok(NULL, ""));
	}
	else //if there is no entry (double commas), remove the comma
		*line = '0';
	return result;
}