/*
 * uart.c
 *
 *  Created on: Apr 1, 2024
 *      Author: akash
 */

#include <stdio.h>
#include "uart.h"

#define TX_PIN	2
#define RX_PIN	3
#define USART_ALT_FN	7

void UartInit(uint32_t baud)
{
	//uart gpio init --- PA.2 (tx) , PA.3 (Rx)
	// enable clock
	RCC->AHB1ENR |= BV(RCC_AHB1ENR_GPIOAEN_Pos);
	// alt fn = 0111 = 7 (tx and rx)
	//AFR[0] ZERO INDEX ELEMWNT OF ARRAY
	GPIOA->AFR[0] |= ((USART_ALT_FN << GPIO_AFRL_AFSEL3_Pos) | (USART_ALT_FN << GPIO_AFRL_AFSEL2_Pos));
	// mode=10 (alt fn), speed=00 (low), pupdr=00 (none), typer= 0(push pull)
	GPIOA->MODER |= (BV(TX_PIN * 2 + 1) | BV(RX_PIN *2 + 1));
	GPIOA->MODER &= ~(BV(TX_PIN * 2) | BV(RX_PIN *2));
	GPIOA->OSPEEDR &= ~(BV(TX_PIN * 2) | BV(TX_PIN *2 + 1) | BV(RX_PIN * 2) | BV(RX_PIN *2 + 1));
	GPIOA->PUPDR &= ~(BV(TX_PIN * 2) | BV(TX_PIN *2 + 1) | BV(RX_PIN * 2) | BV(RX_PIN *2 + 1));
	GPIOA->OTYPER &=~ (BV(TX_PIN) | BV(RX_PIN));


	//UART CONFIG
	// ENABLE UART CLOCK
	RCC->APB1ENR |= BV(RCC_APB1ENR_USART2EN_Pos);
	// TX EN (TE=1), RX EN (RE=1), WORDLEN=8 (M=0), OVER8=0, DISABLE (PCE=0)
	USART2->CR1 = BV(USART_CR1_TE_Pos) | BV(USART_CR1_RE_Pos);
	// 1 STOP BIT (STOP=00), DISABLE CLOCK (CLKEN=0)
	USART2->CR2 = 0x00000000;
	// NO HW CONTROL , NO IRDA, NO DMA, NO INTERRUPTS
	USART2->CR3 = 0X00000000;
	// SET BRR FOR GIVEN BAUD RATE
	switch(baud)
	{
		case BAUD_9600:
			USART2->BRR = BAUD_BRR_9600;
			break;
		case BAUD_38400:
			USART2->BRR = BAUD_BRR_38400;
			break;
		case BAUD_115200:
			USART2->BRR = BAUD_BRR_115200;
			break;
	}
	// ENABLE UART INTERRUPT IN NVIC
	NVIC_EnableIRQ(USART2_IRQn);
	// UART ENABLE (UE=1)
	USART2->CR1 |= BV(USART_CR1_UE_Pos);
}

char *tx_string;
int tx_index = 0;
int tx_complete = 0;

void UartPutch(uint8_t ch)
{
	// WAIT UNTIL TDR IS NOT EMPTY (I.E. PREV BYTE RECEIVED)
	while((USART2->SR & BV(USART_SR_TXE_Pos))== 0)
		;
	// READ RECEIVED BYTE FROM RDR
	USART2->DR = ch;
}

void UartPuts(char str[])
{
	int i;
		for(i=0; str[i] !='\0'; i++)
			UartPutch(str[i]);
}

void USART2_IRQHandler(void)
{
//CHECK IF LAST CHAR TX
	if ((USART2->SR & BV(USART_SR_TXE_Pos)) != 0)
	{
		//SEND NEXT CHAR IF AVAILABLE
		if(tx_string[tx_index] != '\0')
		{
			USART2->DR = tx_string[tx_index];
			tx_index++;
		}
		else
		{
			tx_string = NULL;
			tx_complete = 1;
			//DISABLE UART TXEIE INTERRUPT
			USART2->CR1 &= ~BV(USART_CR1_TXEIE_Pos);
		}

	}
}



