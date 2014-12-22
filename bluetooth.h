/*!
 * @file bluetooth.h
 * @version 1.0
 * @date 2014-12-21
 * @brief Biblioteka do obslugi modulu bluetooth dla plytki Freescale FRDM KL46Z
 *
 */
#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_																			/**< Zabezpieczenie przed wielokrotnym dolaczaniem naglowka*/

#define BAUD_RATE 19200																		/**< Predkosc transferu */
#define BUFF_SIZE 128																			/**< Zadeklarowany rozmiar buforow */

#define OVERWRITE 1

/** Struktura buforow cyklicznych nadajnika i odbiornika */
typedef struct {
	uint8_t buf[BUFF_SIZE];																	/**< Rozmiar bufora */
	uint16_t head;																						/**< Indeks na ostatnie zajete miejsce w buforze */
	uint16_t tail;																						/**< Indeks na pierwszy wolny element bufora */
	uint16_t size;																						/**< Ilosc zajetych elementow */
} UART_BUF_t;

extern volatile UART_BUF_t RxBuf, TxBuf;

/// @brief Funkcja inicjalizacyjna transmisji
void bt_init( uint32_t baud_rate );													/**< Funkcja inicjalizacyjna transmisji */
uint8_t bt_sendChar( char data );													/**< Funkcja wysylajaca jeden bajt (dodajaca jeden bajt do bufora cyklicznego) */
uint8_t bt_sendStr( const char * source );								/**<  */
char bt_getChar( void );																	/**< Funkcja pobierajaca z bufora jeden bajt */
void bt_getStr( char * destination );											/**<  */
void buf_clear( volatile UART_BUF_t * b );
uint8_t buf_empty( volatile UART_BUF_t * b );
uint8_t buf_full( volatile UART_BUF_t * b );
uint8_t to_UART_buffer( const char c, volatile UART_BUF_t * b );
char from_UART_buffer( volatile UART_BUF_t * b );
uint8_t overwrite_UART_buffer( const char c, volatile UART_BUF_t * b );

#endif
