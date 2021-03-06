/**
 * IMT - Rafael Corsi
 * 
 * PIO - 07
 *  Configura o PIO do SAM4S (Banco A, pino 19) para operar em
 *  modo de output. Esse pino est� conectado a um LED, que em 
 *  l�gica alta apaga e l�gica baixa acende.
*/

#include <asf.h>
#include "Driver/pio_maua.h" //Inclui Biblioteca do PIO.

/*
 * Prototypes
 */

/** 
 * Defini��o dos pinos
 * Pinos do uC referente aos LEDS.
 *
 * O n�mero referente ao pino (PIOAxx), refere-se ao
 * bit que deve ser configurado no registrador para alterar
 * o estado desse bit espec�fico.
 *
 * exe : O pino PIOA_19 � configurado nos registradores pelo bit
 * 19. O registrador PIO_SODR configura se os pinos ser�o n�vel alto.
 * Nesse caso o bit 19 desse registrador � referente ao pino PIOA_19
 *
 * ----------------------------------
 * | BIT 19  | BIT 18  | ... |BIT 0 |
 * ----------------------------------
 * | PIOA_19 | PIOA_18 | ... |PIOA_0|
 * ----------------------------------
 */
#define PIN_LED_BLUE 19
#define PIN_LED_GREEN 20
#define PIN_LED_RED 20
#define BTN2 3

#define set(REG, x)	REG |= 1 << x; // defini��o padr�o para setar bits

/**
 * Main function
 * 1. Configura o clock do sistema
 * 2. Desabilita watchdog
 * 3. Ativa o clock para o PIOA
 * 4. Ativa o controle do pino ao PIO
 * 5. Desabilita a prote��o contra grava��es do registradores
 * 6. Ativa a o pino como modo output
 * 7. Coloca o HIGH no pino
 */

int main (void)
{

	/**
	* Inicializando o clock do uP
	*/
	sysclk_init();
	
	/** 
	
	*  Desabilitando o WathDog do uP
	*/
	WDT->WDT_MR = WDT_MR_WDDIS;
		
	// 29.17.4 PMC Peripheral Clock Enable Register 0
	// 1: Enables the corresponding peripheral clock.
	// ID_PIOA = 11 - TAB 11-1
	PMC->PMC_PCER0 |= 1 << ID_PIOA;
	PMC->PMC_PCER0 |= 1 << ID_PIOC;
	PMC->PMC_PCER0 |= 1 << ID_PIOB;
	
	_pio_set_output(PIOA, ((1 << PIN_LED_BLUE ) | (1 << PIN_LED_GREEN )), 0, 0);
	_pio_set_output(PIOB,(1 << PIN_LED_RED ), 0, 0);

	/*
	 //31.6.1 PIO Enable Register
	// 1: Enables the PIO to control the corresponding pin (disables peripheral control of the pin).	
	PIOA->PIO_PER |= (1 << PIN_LED_BLUE ) | (1 << PIN_LED_GREEN );
	PIOC->PIO_PER |= (1 << PIN_LED_RED );
	// 31.6.46 PIO Write Protection Mode Register
	// 0: Disables the write protection if WPKEY corresponds to 0x50494F (PIO in ASCII).
	PIOA->PIO_WPMR = 0;
	PIOC->PIO_WPMR = 0;
	// 31.6.4 PIO Output Enable Register
	// value =
	//	 	1 : Enables the output on the I/O line.
	//	 	0 : do nothing
	PIOA->PIO_OER =  (1 << PIN_LED_BLUE ) | (1 << PIN_LED_GREEN );
	PIOC->PIO_OER =  (1 << PIN_LED_RED );
	// 31.6.10 PIO Set Output Data Register
	// value = 
	// 		1 : Sets the data to be driven on the I/O line.
	// 		0 : do nothing
	PIOA->PIO_SODR = (1 << PIN_LED_BLUE ) | (1 << PIN_LED_GREEN );
	PIOC->PIO_SODR = (1 << PIN_LED_RED );
*/
	//Configura��o do PB3 (BTN2) como entrada	
	PIOB->PIO_PER |= (1 << BTN2); //Habilita o MUX para desabilitar o pino PB3 como sa�da
	PIOB->PIO_ODR |= (1 << BTN2); // Desabilita o buffer do pino PB3
	
	//Configurando Resistor de PulluP
	//PIOB->PIO_PUER = (1 << BTN2);  //Habilita PullUp
	_pio_pull_up(PIOB, BTN2, BTN2);
	PIOB->PIO_PPDDR = (1 << BTN2);	//Desabilita PullDown
	
	//Configura��o de perif�rico de DEBOUNCING
	PIOB->PIO_IFSCDR = (1 << BTN2); //Seta MUX do Peripheral Clock para o Debouncing Filter
	PIOB->PIO_IFER = (1 << BTN2); //Seta MUX do Sinal de entrada para o Debouncing
	
	/**
	*	Loop infinito
	*/
		while(1){

            /*
             * Utilize a fun��o delay_ms para fazer o led piscar na frequ�ncia
             * escolhida por voc�.
             */
			
			if (((PIOB->PIO_PDSR >> BTN2) & 1)){
				PIOA->PIO_SODR =  (1 << PIN_LED_BLUE ); //led azul apaga
			} else {
				PIOA->PIO_CODR =  (1 << PIN_LED_BLUE ); //led azul acende
			}
			 
			
			/*
			PIOC->PIO_SODR |=  (1 << PIN_LED_RED ); //led red acende
			delay_ms(1000);
			PIOC->PIO_CODR |=  (1 << PIN_LED_RED ); //led red apaga
			PIOA->PIO_CODR |=  (1 << PIN_LED_BLUE ); //led azul acende
			delay_ms(1000);
			PIOA->PIO_SODR |=  (1 << PIN_LED_BLUE ); //led azul apaga
			PIOA->PIO_CODR |=  (1 << PIN_LED_GREEN ); //led verde acende
			delay_ms(1000);
			PIOA->PIO_SODR |=  (1 << PIN_LED_GREEN ); //led verde apaga   
			*/   
			
		
	}
}
