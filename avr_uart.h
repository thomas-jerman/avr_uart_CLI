#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

// Makro for using the receive interrupt enable bit (RXCIE0) as an indicator for
// successful command reception. RXCIE0 is disabled in ISR(USART_RX_vect)
// once '\n' has been received or the buffer has got full.
#define UART_CMD_RECEIVED   !(UCSR0B & (1 << RXCIE0))

// Type definition for parity settings
typedef enum
{
    PARITY_NO = 0,
    PARITY_EVEN = 2,
    PARITY_ODD = 3
} UARTParity;

// Initialize UART with custom bitrate and partiy setting
// 8 bits of data and one stop bit
void initUART(uint32_t bps, uint8_t parity);

// Configure stdin and stdout to be connected to UART
void configSTDIO();

// Adopted fflush to be used in the makro fflush for compatibility
// Gets rid of all left over characters before new characters are expected
#define fflush fflushUART
void fflushUART(FILE *uart_stdin);

// Configure standard output stream to use USART
int uart_putchar(char c, FILE *stream);

// Configure standard input stream to use USART
int uart_getchar(FILE *stream);

// Get command from reveived string
char *getUARTCmd();

// Get parameter on each consecutive call of this function
// make sure to check if param != NULL on each function call
char *getUARTParam();

// Reshape UART buffer after command and parameters have been read
// Set all '\0' introduced by strtok back to ' '
void reshapeUARTbuffer();

// Process and print received command and all parameters available
unsigned char processCMD(unsigned char print);

// Reset UART to enable new ISR controlled data reception and
// print UART prompt to let the user know a new command can be received
void printUARTPrompt(char prompt[]);

#endif // UART_H_INCLUDED