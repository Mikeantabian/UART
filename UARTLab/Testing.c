/*--------------------------INCLUDE HEADER FILES---------------------*/
#include "TM4C123GH6PM.h"
# include <stdbool.h>
# include <stdio.h>
#include <ctype.h>

/*--------------------------USER FUNCTION DECLARATIONS---------------*/
/*Function 1: Delay in milliseconds (50 MHz CPU Clock)*/
void delayMs(int ms);
/*Function 2: UART handler*/
void UART0_Handler(void);
/*Function 3: Trasmit a Char*/
void UART0_TransmitChar(char c);
/*Function 4: Transmit a String*/
void UART0_TransmitString(const char * str);

#define BaudRate 115200
#define SysClock 16000000
volatile char c = ' ';
volatile char U = ' ';

/*--------------------------MAIN FUNCTION-----------------------------*/
int main(void){
	/*------------------------INITIALIZE /CONFIGURE PERIPHERALS ----------*/
	/*Such as GPIO, UARTs, ADC, Timer, Interrupt, etc*/
	
	/*  */
	int potent_dig, photosensor_dig, temperature_dig;
	
	/* Clock enables */
	SYSCTL->RCGCUART |= 1; /* Enable clock to UART */
	SYSCTL->RCGCADC |= 1; /* Enable clock to ADC module 0 */
	SYSCTL->RCGCGPIO |= 0x10; /* Enable clock to GPIOE */
	SYSCTL->RCGCGPIO |= 0x01; /* Enable clock to GPIOA */
	
	/* Configure ADC */
	/* Enable alternate functionality  of PE1(light sensor), 
	PE2(potentiometer), PE5(temperature sensor) */
	GPIOE-> AFSEL |= 0x26; 
	GPIOE->DEN &= ~0x26; /* disabling the digital functionality */
	GPIOE->AMSEL |= 0x26; /* enabling analog functionality */
	
	ADC0->ACTSS &= ~1; /* Disable the sample sequencer */
	ADC0->EMUX &= ~0x000F; /* clearing the 4 LSB enables the software trigger */
	ADC0->SSMUX0 &= ~0xFFFFFFFF; /* Clear all the bits to ensure a clean slate */
	ADC0->SSMUX0 |= 0x00000821; /* Choose channel 1 2 and 8 for the samples */
	ADC0->SSCTL0 |= 0x00000600; /* raw interrupt at third samples conversion */
	ADC0->ACTSS |= 1; /* Enable the sample sequencer */
	
	/* Configure the UART */
	GPIOA->AFSEL |= 3; /* Enable alternate functionality of PA1-0 */
	GPIOA->DEN |= 3; /* Enable digital functionality of PA1-0 */
	GPIOA->PCTL |= 17; /* Set high to make UART the alternate function */
	UART0->CTL &= ~1; /* Disable UART before modifying */
	
	//UART0->IBRD = SysClock/(16*BaudRate); /* decimal part of Baud rate */
	//UART0->FBRD = 11; /* fractional part of Baud rate */
	
	UART0->IBRD = 27; /* decimal part of Baud rate  */
	UART0->FBRD = 8; /* fractional part of Baud rate */
	
	UART0->CC = 0; /* Use system clock for UART0 module */
	UART0->LCRH = 0x60; /* 8-bit, no parity, 1-stop bit, no FIFO */
	UART0->CTL |= (1<<0)|(1<<8)|(1<<9); /* enable UART0, TXE, RXE */
	delayMs(50); /* delay for 50ms */
	UART0->DR = 0; /* clear the UART0 data register */
	UART0->IM |= (1<<4); /* Unmask the RX interrupt at bit 4 */
	NVIC->IP[5] = 3 << 5; /* Set the RX handler as priority 3 */
	NVIC->ISER[0] = 1 << 5; /* enable the RX interrupt service */
	delayMs(10);
	/*  */

	/*--------------------------Main Tasks----------------------*/
	/*To keep the program running*/
	/*Main logic of the program goes inside the loop */
	

	double Temp_reading, Potent_reading, Light_reading;
	bool format = true;
	char reading_str[20] = {};

	UART0_TransmitString("\033[2J"); /* Clear terminal screen */
	UART0_TransmitString("\033[H"); /* Set cursor to home */
	UART0_TransmitString("\x1B[?25l"); /* Hide cursor */
	 
		
	for(;;){
		
		ADC0-> PSSI |= 1;
		while ((ADC0->RIS & 1) == 0); /* Do all three sample conversions */
		/* Save digital values from conversion to int varaiables */
		potent_dig = ADC0 -> SSFIFO0;
		photosensor_dig = ADC0 -> SSFIFO0;
		temperature_dig = ADC0 -> SSFIFO0;
		ADC0->ISC = 1; /* Clear interrupt flag */

		U = ((U +1) %9) + 48;
		UART0_TransmitString("\x1B[38;5;");
		UART0_TransmitChar(U);
		UART0_TransmitChar('m');
		
		UART0_TransmitString("\n\n\n\n\n     ");
		
		switch(tolower(c)) {
			case 'v': /* Potentiometer */
				Potent_reading = (potent_dig * 3.3) / 4095; /* Convert value from Digital to Analog */
				sprintf(reading_str, "%.3f", Potent_reading); /* store char reading in array */
				
				UART0_TransmitString("Potentiometer Reading: "); /* Potentiometer */
				UART0_TransmitString(reading_str); /* Potentiometer Values */
				UART0_TransmitString(" V"); /* Potentiometer Unit */
				delayMs(50);

				break;
			case 'l': /* Light sensor */
				Light_reading = (photosensor_dig * 3.3) / 4095; /* Convert value from Digital to Analog */
				sprintf(reading_str, "%.3f", Light_reading); /* store char reading in array */
			
				UART0_TransmitString("Photosensor Reading: "); /* Photosensor */
				UART0_TransmitString(reading_str); /* Photosensor values */
				UART0_TransmitString(" V"); /* Photosensor Unit */
				delayMs(50);

				break;
			case 't': /* Temperature sensor */
				Temp_reading = (temperature_dig * 3.3) / 4095;
				Temp_reading = Temp_reading * 100;
			
				if(format){
					Temp_reading = (Temp_reading * 9.0 / 5.0) + 32.0;
				}
			
				sprintf(reading_str, "%.1f", Temp_reading); /* store char reading in array */
			
				
				UART0_TransmitString("Temperature Reading: "); /* Temperature sensor */
				if(format){
					UART0_TransmitString(reading_str);
					UART0_TransmitString(" F"); /* Temperature Unit in Fahrenheit */
					delayMs(50);
				} else {
					UART0_TransmitString(reading_str);
					UART0_TransmitString(" C"); /* Temperature Unit in Celsius */
					delayMs(50);
				}
				break;
			case 'q': /* Menu screen */
				UART0_TransmitString("MAIN MENU \n\n\r");
				delayMs(50);
				UART0_TransmitString("Press V for Potentiometer \n\r");
				delayMs(50);
				UART0_TransmitString("Press L for Light sensor \n\r");
				delayMs(50);
				UART0_TransmitString("Press T for Temperature \n\r");
				delayMs(50);
				UART0_TransmitString("Press F to change temperature unit \n\r");
				delayMs(50);
				UART0_TransmitString("Press Q to return to this menu \n\r");
				delayMs(50);
				break;
			case 'f': /* Change temoerature format */
				format = !format;
				break;
			default:
				UART0_TransmitString("INVALID OPTION \n\n\r"); /* Error message */
				UART0_TransmitString("**Press Q to return to Main Menu**"); /* Menu return */
		}
		
		delayMs(100);
		//UART0_TransmitString("\033[2J"); /* Clear terminal screen */
		UART0_TransmitString("\033[H"); /* Set cursor to home */
	}
											
}


/*-------------------USER FUNCTION DEFINITIONS---------------------------*/
/*Function 1: Delay in milliseconds (50 MHz CPU Clock)*/
void delayMs(int ms) {
	int i, j;
	for(i = 0 ; i < ms; i++)
		for(j = 0; j < 2850; j++)
			{}   /* do nothing for 1 ms */
}

/*Function 2: UART handler*/
void UART0_Handler(void){
	 /* Check if intterupt is caused by RX */
	if(UART0->MIS & (1<<4)){
		
		c = UART0->DR; /* Read character from data register */
		UART0_TransmitString("\033[2J"); /* Clear terminal screen */
		//UART0_TransmitChar(c); /* Transmit character read */
		
		UART0->ICR |= (1<<4); /* Clear the interrupt */
	}
}

/*Function 3: Trasmit a Char*/
void UART0_TransmitChar(char c){
	/* Wait until the transmit buffer is not full */
	while((UART0->FR & (1<<5)) != 0){}
	UART0->DR = c; /* Transmit the character */
}


/*Function 4: Transmit a String*/
void UART0_TransmitString(const char * str){
	/* While not at end of String line */
	while( *str != '\0') {
		UART0_TransmitChar( *str++); /* print the character being pointed to 
		and increment pointer index */
	}
}

