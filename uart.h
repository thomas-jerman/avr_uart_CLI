#ifndef UART_HEADER_
#define UART_HEADER_

#define RECEIVE_BUFFER 80
#define UART0_CMD_RECEIVED !(UCSR0B & (1 << RXCIE0))

typedef enum
{
    PARITY_NO = 0,
    PARITY_EVEN = 2,
    PARITY_ODD = 3
}UARTParity;

struct uart
{
    int bytes_received;
    char receive_buffer[RECEIVE_BUFFER];
};

void initUART(uint32_t bps, UARTParity parity);

void configSTDIO();

#define fflush fflushUART
void fflushUART(FILE *uart_stdin);

// Configure standard output stream to use USART
int uart_putchar(char c, FILE *stream);

// Configure standard input stream to use USART
int uart_getchar(FILE *stream);

char *getUARTCommand();

char *getUARTParameter();

void resetUART();

void resetUARTbuffer();

void showCmdString();

#endif // UART_HEADER_