#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "global.h"
#include "avr_uart.h"
#include "kbinput.h"

#define BUFFER_LENGTH 	80
#define STD_PROMPT		"IO>"

unsigned char gl_uc_logged_in = 0;

int main()
{
	char text[BUFFER_LENGTH];
	double d_val = M_PI;
	int int_val;
	char *cmd;

	configSTDIO();					// Configure stdin and stdout to be connected to UART
	initUART(115200, PARITY_NO);	// Initialize UART with custom bitrate and partiy setting unsing 8 bit data, 1 stop bit
	printf("\nUART Command Line Interface\nCompiled on: %s at %s\n\n", __DATE__, __TIME__);
	printUARTPrompt(STD_PROMPT); 	// print UART prompt to show, that the ISR-driven UART interface is available

	while (1)
	{
		if (UART_CMD_RECEIVED)		// If receive interrupt enable bit (RXCIE0) is deactivated by ISR, a recevied command line can be processed
		{
			printCmd(); 			// print received command and parameters

			// Command processing
			if ((cmd = getUARTCmd()) == NULL)
				printf("\n");

			// Login
			else if (strcmp(cmd, "login") == 0)
			{
				fflush(stdin);
				getString("Password: ", BUFFER_LENGTH, text);
				if (strcmp(text, "FIVU") == 0)
				{
					printf("Log in successful\n");
					gl_uc_logged_in = 1;
				}
				else
					printf("Wrong password\n");
			}

			// Get Integer Value
			else if (strcmp(cmd, "giv") == 0)
			{
				printf("Get integer Value: ");
				fflush(stdin);
				if (scanf("%d", &int_val) < 1)
					printf("No valid integer value read!\n");
				else
					printf("Integer Value: %d\n", int_val);
			}

			// Get Double Value
			else if (strcmp(cmd, "gdv") == 0)
			{
				printf("Get Double Value: ");
				fflush(stdin);
				if (scanf("%lf", &d_val) < 1)
					printf("No valid double value read!\n");
				else
					printf("Double Value: %lf\n", d_val);
			}

			// Get Word until first space character occures
			else if (strcmp(cmd, "gw") == 0)
			{
				printf("Get Word: ");
				fflush(stdin);
				scanf("%s", text);
				printf("Word: \"%s\"\n", text);
			}

			// Get String including spaces and '\n'
			else if (strcmp(cmd, "gs") == 0)
			{
				printf("Get String: ");
				fflush(stdin);
				fgets(text, BUFFER_LENGTH, stdin);
				printf("String: \"%s\"\n", text);
			}

			// Get String including spaces but not '\n'
			else if (strcmp(cmd, "GS") == 0)
			{
				fflush(stdin);
				getString("Get String (without '\\r\\n'): ", BUFFER_LENGTH, text);
				printf("\nString (without '\\r\\n'): \"%s\"\n", text);
			}			

			// Get limited integer input
			else if (strcmp(cmd, "gliv") == 0) // keyboard input functions offered by kbinput.h
			{
				int int_min, int_max;
				int_min = atoi(getUARTParam());
				int_max = atoi(getUARTParam());
				sprintf(text, "Get Limited Integer Value [%d-%d]: ", int_min, int_max);
				fflush(stdin);
				int_val = getInteger(text, int_min, int_max);
				printf("Limited Integer Value: %d\n", int_val);
			}

			// Get limited double input
			else if (strcmp(cmd, "gldv") == 0) // keyboard input functions offered by kbinput.h
			{
				double double_min, double_max;
				double_min = atoi(getUARTParam());
				double_max = atoi(getUARTParam());
				sprintf(text, "Get Limited Double Value [%.1lf-%.1lf]: ", double_min, double_max);
				fflush(stdin);
				d_val = getDouble(text, double_min, double_max);
				printf("Limited Double Value: %lf\n", d_val);
			}
			else
				printf("Command: [%s] not defined.\n", cmd);

			// additional useful functions
			
			fflush(stdin);
			hitAnyKeyToContinue(stdin);
			if(readYesNo("Enter new command [y/n]?") == 'y')
			{
				printUARTPrompt("CMD>");	// printUARTPrompt() to be used to re-enable interrupt controlled character reception
				while(!UART_CMD_RECEIVED);
				printCmd(stdin); 				// print received command and parameters
				hitAnyKeyToContinue(stdin);
			}

			printUARTPrompt(STD_PROMPT);	//to be used to re-enable interrupt controlled character reception
		}
	}
}