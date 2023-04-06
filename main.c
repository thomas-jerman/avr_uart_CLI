#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "global.h"
#include "uart.h"
#include "kbinput.h"

#define BUFFER_LENGTH 80

int main()
{
	int int_min, int_max;
	char text[BUFFER_LENGTH];
	double d = M_PI;
	//double double_min, double_max;
	int a;
	char *cmd;
	//char *param;

	initUART(115200, PARITY_NO);
	configSTDIO();

	while (1)
	{
		if (UART0_CMD_RECEIVED)
		{
			showCmdString();
			cmd = getUARTCommand();

			if(strcmp(cmd, "giv") == 0)
			{
				printf("Get integer Value: ");
				fflush(stdin);
				if (scanf("%d", &a) < 1)
					printf("No valid integer value read!\n");
				else
					printf("Integer Value: %d\n", a);
			}
			else if(strcmp(cmd, "gdv") == 0)
			{
				printf("Get Double Value: ");
				fflush(stdin);
				if (scanf("%lf", &d) < 1)
					printf("No valid double value read!\n");
				else
					printf("Double Value: %lf\n", d);
			}
			else if(strcmp(cmd, "gw") == 0)
			{
				printf("Get Word: ");
				fflush(stdin);
				scanf("%s", text);
				printf("Word (without whitespaces): %s\n", text);
			}
			else if(strcmp(cmd, "gs") == 0)
			{			
				printf("Get String (with '\\n'): ");
				fflush(stdin);
				fgets(text, BUFFER_LENGTH, stdin);
				printf("String (with whitespaces): %s", text);
			}
			else if(strcmp(cmd, "gliv") == 0)	// keyboard input functions offered by kbinput.h
			{			
				int_min = atoi(getUARTParameter());
				int_max = atoi(getUARTParameter());
				sprintf(text, "Get Limited Integer Value [%d-%d]: ", int_min, int_max);
				fflush(stdin);
				a = getInteger(text, int_min, int_max);
				printf("Limited Integer Value: %d\n", a);
			}
			else if(strcmp(cmd, "gldv") == 0)	// keyboard input functions offered by kbinput.h
			{
				/*
				double_min = atof(getUARTParameter());
				double_max = atof(getUARTParameter());
				sprintf(text, "Get Limited Double Value [%.1lf-%.1lf]: ", double_min, double_max);
				fflush(stdin);
				d = getDouble(text, double_min, double_max);
				printf("Limited Double Value: %lf\n", d);
				*/
				
			}
			else if(strcmp(cmd, "GS") == 0)
			{
				fflush(stdin);
				getString("Get String (without '\\n'): ", BUFFER_LENGTH, text);
				printf("TEXT WITH WHITESPACES: %s\n", text);
			}
			
			hitAnyKeyToContinue(stdin);
			resetUART();
		}
	}
}