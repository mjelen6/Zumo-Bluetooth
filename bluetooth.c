/*!
 * @file bluetooth.c
 * @version 1.0
 * @date 2014-12-21
 * @brief Biblioteka do obslugi modulu bluetooth dla plytki Freescale FRDM KL46Z
 *
 * Biblioteka do obslugi modulu bluetooth dla plytki Freescale FRDM KL46Z
 */

#include "MKL46Z4.h"
#include "bluetooth.h"
#include <string.h>

volatile UART_BUF_t RxBuf, TxBuf;

void UART0_IRQHandler(void){
	
	if(UART0->S1 & UART_S1_RDRF_MASK){
		if( !buf_full(&RxBuf) ) to_UART_buffer( UART0->D, &RxBuf );
#if OVERWRITE==1
		else	overwrite_UART_buffer( UART0->D, &RxBuf );
#endif
	}
	
	if(UART0->S1 & UART_S1_TDRE_MASK){
		if( buf_empty(&TxBuf) )	UART0->C2 &= ~UART_C2_TIE_MASK;
		else	UART0->D = from_UART_buffer( &TxBuf );
	}
}


/**
	Initialize UART0 module. Setting Baud Rate.
*/
void bt_init( uint32_t baud_rate ){

	uint32_t divisor;

	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; // PTA14(TX) PTA15(RX) on mux(3) will be used.

	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2);
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;

	PORTA->PCR[14] |= PORT_PCR_MUX(3);
	PORTA->PCR[15] |= PORT_PCR_MUX(3);

	UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	divisor = (8000000/baud_rate)/16;
	UART0->BDH &= ~UART_BDH_SBR_MASK;
	UART0->BDL &= ~UART_BDL_SBR_MASK;	
	UART0->BDH |= UART_BDH_SBR(divisor>>8);
	UART0->BDL |= UART_BDL_SBR(divisor);
	
	UART0->BDH &= ~UART_BDH_SBNS_MASK;
	
	UART0->C1 &= ~UART_C1_M_MASK;
	UART0->C1 &= ~UART_C1_PE_MASK;
	
	NVIC_SetPriority(UART0_IRQn, 1);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	UART0->C2 |= (UART_C2_TIE_MASK | UART_C2_RIE_MASK);
	
	buf_clear(&TxBuf);
	buf_clear(&RxBuf);
		
	UART0->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
}


uint8_t bt_sendChar( char data ){
	uint8_t temp = to_UART_buffer( data, &TxBuf );
	
	if( !(UART0->C2 & UART_C2_TIE_MASK) ){
		UART0->D = from_UART_buffer( &TxBuf );
		UART0->C2 |= UART_C2_TIE_MASK;
	}
	return temp;
}


uint8_t bt_sendStr( const char * source ){
	uint8_t i = 0;

	if( strlen(source)+1 > BUFF_SIZE-1-TxBuf.size ) return 1;
	
	while( *(source+i) != '\0' ){
		bt_sendChar( *(source+i) );
		++i;
	}
	bt_sendChar( *(source+i) );
	return 0;
}


char bt_getChar( void ){
	
	return from_UART_buffer( &RxBuf );
}


void bt_getStr( char * destination ){
	
	uint16_t i;
	
	if( buf_empty(&RxBuf) ){
		*destination = '\0';
		return;
	}
	
	for( i=0; i<BUFF_SIZE; ++i){
		if( (RxBuf.buf[i] == 0) || (RxBuf.buf[i] == 13) ) break;
	}
	
	if( i == BUFF_SIZE ) {
		*destination = '\0';
		return;
	}
	else{
		i = 0;
		while( (RxBuf.buf[RxBuf.head] != '\0') && (RxBuf.buf[RxBuf.head] != 13) ){
			*(destination+i) = from_UART_buffer( &RxBuf );
			++i;
		}
		from_UART_buffer( &RxBuf );
		*(destination+i) = '\0';
	}	
}


void buf_clear( volatile UART_BUF_t * b ){
	uint16_t i;
	for(i=0; i<BUFF_SIZE; ++i) b->buf[i] = 0;		
		
	b->head = 0;
	b->tail = 0;	
	b->size = 0;	
}


uint8_t buf_empty( volatile UART_BUF_t * b ){
	
	if( b->size == 0 ) return 1;
	else return 0;
}


uint8_t buf_full( volatile UART_BUF_t * b ){
	
	if( b->size+1 == BUFF_SIZE ) return 1;
	else return 0;
}


uint8_t to_UART_buffer( const char c, volatile UART_BUF_t * b ){
	
	if( !buf_full( b ) ){
	
		b->buf[b->tail++] = c;
		b->tail %= BUFF_SIZE;
		++(b->size);
		return 0;
	}
	return 1;
}


char from_UART_buffer( volatile UART_BUF_t * b ){
	
	char c = 0;
	
	if( !buf_empty( b ) ){
	
		c = b->buf[b->head];
		b->buf[b->head++] = 0;
		b->head %= BUFF_SIZE;
		--(b->size);
	}
	return c;
}


uint8_t overwrite_UART_buffer( const char c, volatile UART_BUF_t * b ){

	if( buf_full( b ) ){
	
		b->buf[b->tail++] = c;
		b->tail %= BUFF_SIZE;
		b->head++;
		b->head %= BUFF_SIZE;
		return 0;
	}
	return 1;
}
