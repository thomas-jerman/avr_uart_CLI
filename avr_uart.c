/*  avr_uart.c for UART Communication
    26.06.2023
    Thomas Jerman and Michael Lieschnegg
*/

#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "avr_uart.h"

#define REC_CHAR_MAX    80      // REC_CHAR_MAX: maximum number of receivable characters
#define LF              0x0A    // Line Feed
#define CR              0x0D    // Carriage Return

#define LINE_END        CR
#define SIZE            REC_CHAR_MAX + 1

// Uart struct
struct uart
{
    uint8_t rcv_index;  // receive index
    char rcv_buf[SIZE]; // receive buffer
    char char_rcvd;     // char received
} UART;

// Initialize UART with custom bitrate and partiy setting unsing 8 bit data, 1 stop bit
void initUART(uint32_t bps, uint8_t parity)
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

    // RX Complete Interrupt Enable is set by printUARTPrompt()
    // UCSR0B |= (1 << RXCIE0);

    // set UART.rcv_buf to be zero
    memset(UART.rcv_buf, 0, SIZE);

    // enable global interrupt
    sei(); // is equivalent to  SREG |= SREG_I;
}

// Configure stdin and stdout to be connected to UART
void configSTDIO()
{
    static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
    static FILE uart_stdin = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);
    stdout = &uart_stdout;
    stdin = &uart_stdin;
}

// Adopted fflush to be used in the makro #define fflush fflushUART for compatibility
// Gets rid of all left over characters before new characters are expected
void fflushUART(FILE *uart_stdin)
{
    if (uart_stdin->flags > 1)
        while (getchar() != '\n');
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
    // Save the received byte
    UART.char_rcvd = UDR0;
    // Return the received byte or change it to \n if \r has been received
    return UART.char_rcvd != '\r' ? UART.char_rcvd : '\n';
}

// Get command from reveived string
char *getUARTCmd()
{
    return strtok(UART.rcv_buf, " \r\n");
}

// Get parameter on each consecutive call of this function, which
// processes strings under quotes ss one single string
// Make sure to check if param != NULL after each function call
char *getUARTParam()
{
    char *p = strtok(NULL, " \r\n");    // tokenize on next space character
    if (*p == '"')                      // check for a quote character
    {
        *strchr(p++, '\0') = ' ';       // restore space character
        p = strtok(p, "\"\r\n");        // tokenize on next quote character
    }
    if (p < UART.rcv_buf + SIZE)
        return p;
    else
        return NULL;
}

// Reshape UART buffer after command and parameters have been read
// Set all '\0' introduced by strtok back to ' '
void reshapeUARTbuffer()
{
    for (int i = 0; i < SIZE - 1; i++)
        if (UART.rcv_buf[i] == '\0')
            UART.rcv_buf[i] = ' ';

    if (UART.rcv_index == SIZE - 1)
        while (getchar() != '\n');
}

// Process and print received command and all parameters available
// Input parameter unsigned char print:
// Use #define PRINT_CMD 1 to print received command and parameters
// Use #define NO_PRINT  0 to just process data without printing
unsigned char processCMD(unsigned char print)
{
    //for(int i = 0; i < SIZE -1; i++)    printf("%d ", UART.rcv_buf[i]);   //debug code
    if (UART.rcv_index)
        {
            int param_count = 0;
            char *p;
            if ((p = getUARTCmd()) != NULL && print)
                printf("Command: [%s]\n", p);
            else if (print)
                     printf("No cmd available!");
            do
            {
                if ((p = getUARTParam()) != NULL && p < UART.rcv_buf + SIZE && print)
                    printf("Param#%X: [%s]\n", ++param_count, p);
            } while (p != NULL);
            if (print)
                printf("#ofParams: %d\n", param_count);
            reshapeUARTbuffer();
        return param_count;
    }
    return 0;
}

// Reset UART to enable new ISR controlled data reception and
// print UART prompt to let the user know a new command can be received
void printUARTPrompt(char prompt[])
{
    fflush(stdin);
    memset(UART.rcv_buf, 0, SIZE);
    UART.rcv_buf[SIZE - 1] = '\n';
    UART.rcv_index = UDR0; // read UDR0 to empty it
    UART.rcv_index = UDR0; // read UDR0 to empty it
    UART.rcv_index = 0;
    printf("%s", prompt);
    UCSR0A &= ~(1 << RXC0);
    UCSR0B |= (1 << RXCIE0);
}

// ISR for character reception
ISR(USART_RX_vect)
{
    if (((UART.rcv_buf[UART.rcv_index] = UDR0) == LINE_END || ++UART.rcv_index == SIZE - 1))
        UCSR0B &= ~(1 << RXCIE0);   // disable Receive Interrupt
}