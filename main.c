#include "MKL46Z4.h"
#include "bluetooth.h"
#include "leds.h"
#include "motorDriver.h"
#include "skmj_sLCD.h"


int main(void){

	unsigned char c;
	char tab[20];
	
	uint32_t i=0;
	ledsInitialize();
	sLCD_Init();
	bt_init( BAUD_RATE );
	
	//for (i=0; i<10000000; ++i);
	
	while(1){		
		tab[0] = bt_getChar();
		tab[1] = '\0';
		bt_sendStr(tab);
		
		
		
		//if( c == 13 ) bt_sendChar( '\n');
		for (i=0; i<10000000; ++i);
		sLCD_setByType(RxBuf.size,'d');
		//overwrite_UART_buffer('f',&TxBuf);
	}
}
