#pragma once

#define UART_BASE       0x10000000LL
#define UART_THR_OFFSET 0
#define UART_RBR_OFFSET 0
#define UART_LSR_OFFSET 5

void uart_init();
void uart_write(char c);
char uart_read();
