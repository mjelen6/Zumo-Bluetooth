/**
 * @file		main.c
 * @brief		Example of use of bluetooth library. Testing with Putty terminal. 
 */
#include "MKL46Z4.h"
#include "bluetooth.h"
#include "skmj_sLCD.h"

#include <string.h>


char tab[BUFF_SIZE];			// If your array is big, define it as global.
	
int main(void){

	const char hello[] = "Hello World";
	const char newline[] = "\r\n";
	
	uint16_t i=0;
	
	sLCD_Init();											// Initialize LCD
	sLCD_setByType(8888,'d');					// Test all segments
	bt_init( BAUD_RATE );							// Initialize Bluetooth/UART module
	
	
	for(i=0; hello[i] != '\0'; i++){
		bt_sendChar( hello[i] );					// Send string byte by byte
	}
	bt_sendChar('\r');								// New line in putty terminal
	bt_sendChar('\n');								// \r == 13  \n == 10 in ASCII
	
	bt_sendStr("\r\n");								// It works too.
	
	bt_sendStr("Press any key...");
	bt_sendStr( newline );	
	
	while( bt_getChar() == '\0' ){}		// Wait for any character
	bt_sendStr( newline );	

	bt_sendStr("Type something and press Enter...");
	bt_sendStr( newline );		
		
	while(1){													// Main loop
		
		bt_getStr( tab );								// Get string from buffer
		if(strlen( tab )){							// If isn't empty...
			bt_sendStr( tab );						// ...send it back.
			bt_sendStr("\r\n");
		}
		
		sLCD_setByType(RxBuf.size,'d');	// Show how many characters is in the Rx buffer
	}
}
