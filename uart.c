#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "uart.h"

struct uart UART0;

void initUART(uint32_t bps, UARTParity parity)
{  
    // disable global interrupt
    cli(); // is equivalent to  SREG |= SREG_I;

    // set baud rate
    UBRR0 = (uint16_t)((((F_CPU / (bps * 8UL))) - 1) + 0.5);

    // set double baudrate bit
    UCSR0A |= (1 << U2X0);

    // set asynchronous mode
    UCSR0C &= ~((1 << UMSEL01) | (1 << UMSEL00));

    // set paritiy
    UCSR0C &= ~((1 << UPM01) | (1 << UPM00));
    UCSR0C |= parity;

    // set 1 stopp bit
    UCSR0C &= ~(1 << USBS0);

    // set 8 data bits
    UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00));
    UCSR0B &= ~(1 << UCSZ02);

    // enable transmitter and receiver
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    // RX Complete Interrupt Enable
    UCSR0B |= (1 << RXCIE0);

    // init UART0
    UART0.bytes_received = 0; 

    // enable global interrupt
    sei(); // is equivalent to  SREG |= SREG_I;    
}

void configSTDIO()
{
    static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
    static FILE uart_stdin = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);
    stdout = &uart_stdout;
    stdin = &uart_stdin;
}

void fflushUART(FILE *uart_stdin)
{
    if(uart_stdin->flags > 1)   while (getchar() != '\n');
}

// Configure standard output stream to use UART
int uart_putchar(char send_byte, FILE *stream)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    // Send character
    UDR0 = send_byte;
    return 0;
}

// Configure standard input stream to use UART
int uart_getchar(FILE *stream)
{
    // Wait for character to be received
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;    //return character
}

void resetUART()
{
    memset(UART0.receive_buffer, 0, RECEIVE_BUFFER);
    UART0.bytes_received = UDR0;
    UART0.bytes_received = UDR0;
    UART0.bytes_received = 0;
    UCSR0A &= ~(1 << RXC0);
    UCSR0B |= (1 << RXCIE0);
    //printf("\nIO>");
}

void resetUARTbuffer()
{
    for(int i = 0; UART0.receive_buffer[i] != '\n'; i++) if(UART0.receive_buffer[i] == '\0') UART0.receive_buffer[i] = ' ';
}

void showCmdString()
{
    resetUARTbuffer();
	int int_var = 1;
    char *param;
    printf("Command: [%s]\n", getUARTCommand());
	do
	{
		param = getUARTParameter();
		if (param != NULL)	printf("Param#%d: [%s]\n", int_var++, param);
	} while (param != NULL);
	resetUARTbuffer();
}

char *getUARTCommand()
{
    return strtok(UART0.receive_buffer, " \r\n");
}

char *getUARTParameter()
{
    return strtok(NULL, " \r\n");
}

ISR(USART_RX_vect)
{
    if (((UART0.receive_buffer[UART0.bytes_received] = UDR0) == 0x0A || UART0.bytes_received == RECEIVE_BUFFER - 2))
    {
        if(UART0.bytes_received)
        {
            UCSR0B &= ~(1 << RXCIE0);
            //UART0.receive_buffer[UART0.bytes_received] = 0x0A;
        }
        else UART0.bytes_received = -1;
    }
    UART0.bytes_received++;   
}
