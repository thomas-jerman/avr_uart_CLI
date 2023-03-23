#include <stdio.h>
#include <string.h>
#include <math.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "global.h"
#include "uart.h"
#include "kbinput.h"

#define BUFFER_LENGTH 80

int main()
{
	int int_var;
	int int_min, int_max;
	char text[BUFFER_LENGTH];
	char output[BUFFER_LENGTH];
	double d = M_PI;
	char *cmd, *param;
	int a;

	initUART(57600, PARITY_NO);

	while (1)
	{
		if (UART0_CMD_RECEIVED)
		{
			cmd = getUARTCommand();
			printf("Command: [%s]\n", cmd);
			int_var = 1;
			do
			{
				param = getUARTParameter();
				if (param != NULL)
					printf("Param#%d: [%s]\n", int_var++, param);
			} while (param != NULL);

			// using printf and scanf
			printf("Enter an integer value: ");
			if (scanf("%d", &a) < 1)
				printf("No valid integer value read!\n");
			else
				printf("Integer value: %d\n", a);

			printf("Enter a double value: ");
			while (getchar() != '\n'); // clear input buffer
			if (scanf("%lf", &d) < 1)
				printf("No valid double value read!\n");
			else
				printf("Double value: %lf\n", d);

			printf("Enter a text without whitespaces: ");
			while (getchar() != '\n'); // clear input buffer
			scanf("%s", text);
			printf("Text without whitespaces: %s\n", text);

			printf("Enter a text with whitespaces: ");
			while (getchar() != '\n'); // clear input buffer
			fgets(text, BUFFER_LENGTH, stdin);
			printf("Text with whitespaces: %s", text);

			// keyboard input functions offered by kbinput.h
			int_min = 0;
			int_max = 100;
			sprintf(output, "ENTER AN INTEGER VALUE [%d-%d]: ", int_min, int_max);
			a = getInteger(output, int_min, int_max);
			printf("INTEGER VALUE: %d\n", a);

			d = getDouble("ENTER A DOUBLE VALUE [0.0-10.0]: ", 0, 10);
			printf("DOUBLE VALUE: %lf\n", d);

			getString("ENTER A TEXT WITH WHITESPACES: ", BUFFER_LENGTH, text);
			printf("TEXT WITH WHITESPACES: %s", text);

			hitAnyKeyToContinue();

			if (strcmp(cmd, "readInt") == 0)
			{
				printf("Enter an integer value: ");
				if (scanf("%d", &a) < 1)
					printf("No valid integer value read!\n");
				else
					printf("Integer value: %d\n", a);
			}
			else if (strcmp(cmd, "readDouble") == 0)
			{
				printf("Enter a double value: ");
				//while (getchar() != '\n'); // clear input buffer
				if (scanf("%lf", &d) < 1)
					printf("No valid double value read!\n");
				else
					printf("Double value: %lf\n", d);
			}
			else if (strcmp(cmd, "readText-WS") == 0)
			{
				printf("Enter a text without whitespaces: ");
				//while (getchar() != '\n'); // clear input buffer
				scanf("%s", text);
				printf("Text without whitespaces: %s\n", text);
			}
			else if (strcmp(cmd, "readText+WS") == 0)
			{
				printf("Enter a text with whitespaces: ");
				//while (getchar() != '\n'); // clear input buffer
				fgets(text, BUFFER_LENGTH, stdin);
				printf("Text with whitespaces: %s", text);
			}
			else if (strcmp(cmd, "readLimitedInt") == 0)
			{
				int_min = 0;
				int_max = 100;
				sprintf(output, "ENTER AN INTEGER VALUE [%d-%d]: ", int_min, int_max);
				a = getInteger(output, int_min, int_max);
				printf("INTEGER VALUE: %d\n", a);
			}
			else if (strcmp(cmd, "readLimitedDouble") == 0)

			{
				d = getDouble("ENTER A DOUBLE VALUE [0.0-10.0]: ", 0, 10);
				printf("DOUBLE VALUE: %lf\n", d);
			}
			else if (strcmp(cmd, "getString") == 0)
			{
				getString("ENTER A TEXT WITH WHITESPACES: ", BUFFER_LENGTH, text);
				printf("TEXT WITH WHITESPACES: %s", text);
			}

			hitAnyKeyToContinue();

			resetUART();
		}
	}
}