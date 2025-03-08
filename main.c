/*
 * D2-PROYECTO_01-MASTER.c
 *
 * Created: 2/25/2025 4:44:15 PM
 * Author : samue
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


//	LIBRERIAS USASDAS
#include "I2C/I2C.h"
#include "LCD/LCD.h"
#include "UART/UART.h"
#include "AHT10/AHT10.h"


//	VARIABLES POR USAR
volatile uint8_t data_index = 0; 
uint32_t humidity_raw;
uint32_t temperature_raw;
char buffer[16];
#define SLAVE1_ADDRESS 0x30  // Dirección I2C del esclavo 1
#define SLAVE2_ADDRESS 0x29
uint8_t valorLuz = 0;
uint8_t bufferI2C;
uint8_t valordis = 0;
//uint8_t distancia = 0;
uint8_t temp;
char buffer_UART[50];
volatile char comando_UART = 0;
uint8_t sendDC_I2C = 0;
uint8_t sendSERVO_I2C = 0;
uint8_t HUMEDAD_I2C;
uint8_t bandera_riego = 0;


// declaracion de buffers
char buffer_u_t[30];
char buffer_u_h[30];

char buffer_slave1[30];




//	FUNCIONES 
void setup(){
	cli();
	I2C_Master_Init(100000, 1);
	init_LCD();
	initUART9600();
	AHT10_Init();
	UCSR0B |= (1 << RXCIE0);
	
	sei(); // Habilitar interrupciones globales
	LCD_CURSOR(0, 0);
	LCD_STR("Temp Hum Luz Dis");
	

	
}





uint8_t leerFotoresistencia() {
	// Iniciar comunicación con el esclavo
	PORTB |= (1 << PORTB5);
	I2C_Master_Start();

	// Enviar dirección del esclavo en modo escritura
	bufferI2C = SLAVE1_ADDRESS << 1 & 0b11111110;
	temp = I2C_Master_Write(bufferI2C);
	if(temp != 1) {
		I2C_Master_Stop();
		return 255;  // Si hay error, devolver un valor alto
		} else {
		I2C_Master_Write('R');  // Comando para indicar lectura
		I2C_Master_Stop();
	}
	_delay_ms(10);

	// Leer el dato del esclavo
	PORTB &= ~(1 << PORTB5);
	I2C_Master_Start();

	bufferI2C = SLAVE1_ADDRESS << 1 | 0b00000001;  // Dirección en modo lectura
	temp = I2C_Master_Write(bufferI2C);
	if(temp != 1) {
		I2C_Master_Stop();
		return 255;
		} else {
		TWCR = (1 << TWINT) | (1 << TWEN);
		while (!(TWCR & (1 << TWINT)));

		valorLuz = TWDR;  // Guardar el valor recibido
		//luz_uart = valorLuz;
		I2C_Master_Stop();
	}

	return valorLuz;
}




uint8_t leerDistancia() {
	I2C_Master_Start();

	bufferI2C = SLAVE2_ADDRESS << 1 | 0b00000001;  // Dirección en modo lectura
	temp = I2C_Master_Write(bufferI2C);
	if (temp != 1) {
		I2C_Master_Stop();
		return 255;  // Error en la comunicación
		} else {
		TWCR = (1 << TWINT) | (1 << TWEN);
		while (!(TWCR & (1 << TWINT)));

		valordis = TWDR;  // Guardar el valor recibido
		I2C_Master_Stop();
	}

	return valordis;
}

void procesarComandoUART(char comando){
		if (comando == 'S'){
			
				sendSERVO_I2C = 1;
				I2C_Master_Start();
				I2C_Master_Write((SLAVE1_ADDRESS << 1) & 0b11111110);
				I2C_Master_Write(sendSERVO_I2C);
				I2C_Master_Stop();
				sprintf(buffer_slave1, "sendSERVO_I2C = %d\r\n", sendSERVO_I2C);
				WriteTextUART(buffer_slave1);
				
				
			} else if (comando == 's'){
				sendSERVO_I2C = 0;
				I2C_Master_Start();
				I2C_Master_Write((SLAVE1_ADDRESS << 1) & 0b11111110);
				I2C_Master_Write(sendSERVO_I2C);
				I2C_Master_Stop();
				sprintf(buffer_slave1, "sendSERVO_I2C = %d\r\n", sendSERVO_I2C);
				WriteTextUART(buffer_slave1);
			} else if (comando == 'R') {
				sendDC_I2C = 1;
				I2C_Master_Start();
				I2C_Master_Write((SLAVE2_ADDRESS << 1) & 0b11111110);
				I2C_Master_Write(sendDC_I2C);
				sprintf(buffer_slave1, "sendDC_I2C = %d\r\n", sendDC_I2C);
				WriteTextUART(buffer_slave1);
				bandera_riego = 1;
				
			} else if (comando == 'r') {
				sendDC_I2C = 0;
				I2C_Master_Start();
				I2C_Master_Write((SLAVE2_ADDRESS << 1) & 0b11111110);
				I2C_Master_Write(sendDC_I2C);
				I2C_Master_Stop();
				sprintf(buffer_slave1, "sendDC_I2C = %d\r\n", sendDC_I2C);
				WriteTextUART(buffer_slave1);
				bandera_riego = 0;
			
			}
	
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											MAIN	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void) {
	
	setup();
	

	while (1) {
		if (comando_UART) {
			procesarComandoUART(comando_UART);
			comando_UART = 0; // Reset para esperar el siguiente comando
		}
		//	LECTURA DEL SENSOR
		AHT10_TriggerMeasurement();
		_delay_ms(500);
		AHT10_RequestData(); 
		_delay_ms(500); 
		
		if (AHT10_IsDataReady()) {
			AHT10_GetRawData(&humidity_raw, &temperature_raw);

			// CONVERSION
			float humidity = ((float)humidity_raw / 1048576.0) * 100.0;
			float temperature = ((float)temperature_raw / 1048576.0) * 200.0 - 50.0;
			
			uint8_t luz = leerFotoresistencia();
			uint8_t distancia = leerDistancia();

			
			LCD_CURSOR(0, 1);
			dtostrf(temperature, 3, 1, buffer);
			LCD_STR(buffer);
			LCD_STR("    ");
			// SEND UART 
			
			dtostrf(temperature, 3, 1, buffer_u_t);
			sprintf(buffer_UART, "t:%s\r\n", buffer_u_t);
			WriteTextUART(buffer_UART);
			
			
			
			LCD_CURSOR(5, 1);
			dtostrf(humidity, 2, 0, buffer);
			LCD_STR(buffer);
			LCD_STR("    ");
			LCD_CURSOR(7, 1);
			LCD_STR("%");
			// SEND UART
			
			dtostrf(humidity, 3, 1, buffer_u_h);
			sprintf(buffer_UART, "h:%s\r\n", buffer_u_h);
			WriteTextUART(buffer_UART);
			
			
			
			LCD_CURSOR(9, 1);
			snprintf(buffer, 4, "%3d", luz);  // Mostrar número con 3 dígitos
			LCD_STR(buffer);
			// SEND UART 
			sprintf(buffer_UART, "l:%d\r\n", luz);
			WriteTextUART(buffer_UART);
			
			// MUESTREO DE DISTANCIA
			
			LCD_CURSOR(13, 1);
			snprintf(buffer, 4, "%3d", distancia);  // Mostrar número con 3 dígitos
			LCD_STR(buffer);
			
			
			sprintf(buffer_UART, "d:%d\r\n", distancia);
			WriteTextUART(buffer_UART);
			
			// PARA DECIR SI DEBE REGAR O NO 
			if (bandera_riego == 1) {
				
				if (sendDC_I2C == 0) {  
					sendDC_I2C = 1;
					I2C_Master_Start();
					I2C_Master_Write((SLAVE2_ADDRESS << 1) & 0b11111110);
					I2C_Master_Write(sendDC_I2C);
					I2C_Master_Stop();
					
					sprintf(buffer_slave1, "sendDC_I2C = %d\r\n", sendDC_I2C);
					WriteTextUART(buffer_slave1);
				}
				} else {
				
				if (humidity < 60) {
					if (sendDC_I2C == 0) {  
						sendDC_I2C = 1;
						I2C_Master_Start();
						I2C_Master_Write((SLAVE2_ADDRESS << 1) & 0b11111110);
						I2C_Master_Write(sendDC_I2C);
						I2C_Master_Stop();
						
						sprintf(buffer_slave1, "sendDC_I2C = %d\r\n", sendDC_I2C);
						WriteTextUART(buffer_slave1);
					}
					} else {
					if (sendDC_I2C == 1) {  
						sendDC_I2C = 0;
						I2C_Master_Start();
						I2C_Master_Write((SLAVE2_ADDRESS << 1) & 0b11111110);
						I2C_Master_Write(sendDC_I2C);
						I2C_Master_Stop();
						
						sprintf(buffer_slave1, "sendDC_I2C = %d\r\n", sendDC_I2C);
						WriteTextUART(buffer_slave1);
					}
				}
			}
			
			
		}

	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								INTERRUPCIONES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ISR(TWI_vect) {
	uint8_t status = TWSR & 0xF8;

	switch (status) {
		case 0x08: // START transmitido
		case 0x10: // REPEATED START transmitido
		TWDR = (AHT10_ADDRESS << 1) | 1; // Dirección del esclavo con bit de lectura
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		break;

		case 0x40: // SLA+R transmitido y ACK recibido
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
		break;

		case 0x50: // Dato recibido con ACK (para los primeros 5 bytes)
		AHT10_StoreReceivedByte(TWDR);
		if (data_index < 5) {
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
			} else {
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE); // Último byte sin ACK
		}
		break;

		case 0x58: // Último dato recibido sin ACK
		AHT10_StoreReceivedByte(TWDR);
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Enviar STOP
		AHT10_SetReadyFlag();
		break;

		default:
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Reiniciar comunicación
		break;
	}
}

ISR(USART_RX_vect) {
	comando_UART = UDR0; 
}

	
	
	
