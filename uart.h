#ifndef UART_HEADER_
#define UART_HEADER_

#define RECEIVE_BUFFER 80
#define UART0_CMD_RECEIVED !(UCSR0B & (1 << RXCIE0))

typedef enum
{
    PARITY_NO = 0,
    PARITY_EVEN = 2,
    PARITY_ODD = 3
} UARTParity;

struct uart
{
    int bytes_received;
    char receive_buffer[RECEIVE_BUFFER];
};

// Initialize USART
void initUART(uint32_t bps, UARTParity parity);

// configure stdin and stdout to use uart_getchar and uart_putchar for UART communication
void configSTDIO();

// Configure standard output stream to use USART
int uart_putchar(char c, FILE *stream);

// Configure standard input stream to use USART
int uart_getchar(FILE *stream);

// Create command token
char *getUARTCommand();

// Create parmeter token
char *getUARTParameter();

// Reset receive_buffer and reactivate RXCIE0
void resetUART();

#endif // UART_HEADER_