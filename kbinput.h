#ifndef KBINPUT_H_INCLUDED
#define KBINPUT_H_INCLUDED

#define STRING_BUFFER_LENGTH 80

int getInteger(char text[],int lower_limit,int upper_limit);
double getDouble(char text[],double lower_limit,double upper_limit);
char* getString(char text[], int chars_to_read, char dest_string[]);
void hitAnyKeyToContinue(FILE *stream);
int readSingleCharacter(char text[]);
int readYesNo(char text[]);

#endif // KBINPUT_H_INCLUDED
