/*  kbinput.c for keyboard input functions
    28.09.2020
    Thomas Jerman
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "kbinput.h"

#define ESC 27

/****************************************************************************/
// int getInteger(char text[],int lower_limit,int upper_limit)
//
// Reads an integer value from keyboard (standard input = std)
// As long as the entered integer value is lower than lower_limit
// or higher than upper_limit, the input is repeated infinitely.
//
// Input parameters:    prompt text describing which value is to be entered,
//                      int lower_limit, int upper_limit
// Return data type:    int
// Return value:        entered integer value
/****************************************************************************/
int getInteger(char text[], int lower_limit, int upper_limit)
{
    int intValue;
    char s[STRING_BUFFER_LENGTH];
    int result;
    do
    {
        printf("%s", text);
        fgets(s, STRING_BUFFER_LENGTH, stdin);
        result = sscanf(s, "%d", &intValue);
    } while (result < 1 || intValue < lower_limit || intValue > upper_limit);
    return intValue;
}

/****************************************************************************/
// int getDouble(char text[],double lower_limit,double upper_limit)
//
// Reads a double value from keyboard (standard input = std)
// As long as the entered double value is lower than lower_limit
// or higher than upper_limit, the input is repeated infinitely.
//
// Input parameters:    prompt text describing which value is to be entered,
//                      double lower_limit, double upper_limit
// Return data type:    double
// Return value:        entered double value
/****************************************************************************/
double getDouble(char text[], double lower_limit, double upper_limit)
{
    double doubleValue;
    char s[STRING_BUFFER_LENGTH];
    int result;
    do
    {
        printf("%s", text);
        fgets(s, STRING_BUFFER_LENGTH, stdin);
        result = sscanf(s, "%lf", &doubleValue);
    } while (result < 1 || doubleValue < lower_limit || doubleValue > upper_limit);
    return doubleValue;
}

/****************************************************************************/
// char *getString(char text[], int chars_to_read, char dest_string[])
//
// Returned string includes spaces but not '\n'
/****************************************************************************/
char *getString(char text[], int chars_to_read, char dest_string[])
{
    char s[STRING_BUFFER_LENGTH];
    if (chars_to_read > STRING_BUFFER_LENGTH)
    {
        printf("Maximum readable characters set to: 80\n");
        chars_to_read = STRING_BUFFER_LENGTH;
    }
    do
    {
        printf("%s", text);
        fgets(s, chars_to_read, stdin); // fgets also reads the newline character
    } while (strlen(s) == 1);
    sscanf(s, "%[^\r\n]s", dest_string);
    return dest_string;
}

/****************************************************************************/
// void hitAnyKeyToContinue(FILE *stream)
//
// Hit any key to continue
/****************************************************************************/
void hitAnyKeyToContinue(FILE *stream)
{
    char single_char;
    printf("Hit any key to continue: ");
    if(stream->flags > 1)   while (getchar() != '\n');
    if((single_char = getchar()) != '\n') while(getchar() != '\n');
    printf("\n");
}

/****************************************************************************/
// int readSingleCharacter(char text[])
//
// Read single character used by int readYesNo(char text[])
/****************************************************************************/
int readSingleCharacter(char text[])
{
    printf("%s", text);
    return tolower(getchar());
}

/****************************************************************************/
// int readYesNo(char text[])
//
// Read Yes or No
/****************************************************************************/
int readYesNo(char text[])
{
    char single_char;
    do
    {
        single_char = readSingleCharacter(text);
        if(single_char != '\n') while(getchar() != '\n');
    } while (single_char != 'y' && single_char != 'n');
    printf("\n");
    return single_char;
}
