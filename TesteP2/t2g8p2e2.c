#include <detpic32.h>
volatile int unsigned v;
volatile int unsigned voltMin = 500;
volatile int unsigned voltMax = 0;

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);	
	}
}

void send2displays(unsigned char value)
{
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	static char displayFlag = 0;
	int dh = value >> 4;
	int dl = value & 0x0F;
	if(displayFlag == 0)
	{
		dl = display7Scodes[dl];
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;
		dl = dl << 8;
		LATB = (LATB & 0x80FF) + dl;
	}
	else
	{
		dh = display7Scodes[dh];
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
		dh = dh << 8;
		LATB = (LATB & 0x80FF) + dh;
	}
	displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
	return(( value / 10) << 4) + (value % 10);
}

void configureAll(void)
{
	// Configure all (digital I/O analog input, A/D module)
	LATB = LATB & 0x80FF;
	TRISB = (TRISB & 0x80FF) | 0x0003;
	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 0;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	// Configure A/D module and Port RB4 as Analog Input
	TRISBbits.TRISB4 = 1;	// RB4 digital output disconnect
	AD1PCFGbits.PCFG4 = 0; 	// RB configured as analog input
	AD1CON1bits.SSRC = 7;	// Conversion trigger select bits
	AD1CON1bits.CLRASAM = 1;// Stop conversion when 1 interrupt
				// is generated from ADC and clears
				// the ASAM bit
	AD1CON3bits.SAMC = 16;	// Sample time is 16 TAD 
				// ( TAD = 100ns)
	AD1CON2bits.SMPI = 7;	// 8-1 samples converted and BUFFERED
	AD1CHSbits.CH0SA = 4;	// Select AN4 as input for ADC
	AD1CON1bits.ON = 1;	// Enable ADC (must be last instr.)
	// Configure interrupt system
	IPC6bits.AD1IP = 2;	// Configure priority of A/D
	IEC1bits.AD1IE = 1;	// enable A/D Interrupts
	IFS1bits.AD1IF = 0;	// Reset AD1IF flag
	// Configure Timer T1 with interrupts enable
	T1CONbits.TCKPS = 3;	// Presc = 256
	PR1 = 19530;
	TMR1 = 0;		// Reset timer T1 count register
	T1CONbits.TON = 1;	// Enable T1 timer
	IPC1bits.T1IP = 2;	// Interrupt Priority
	IEC0bits.T1IE = 1;	// Enable T1 interrupts
	IFS0bits.T1IF = 0;	// Reset Timer T1 IF
	// Configure Timer T3 with interrupts enable
	T3CONbits.TCKPS = 2;	// Presc = 4
	PR3 = 49999;
	TMR3 = 0;		// Reset timer T3 count register
	T3CONbits.TON = 1;	// Enable T3 timer
	IPC3bits.T3IP = 2;	// Interrupt Priority
	IEC0bits.T3IE = 1;	// Enable T3 interrupts
	IFS0bits.T3IF = 0;	// Reset Timer T3 IF
	EnableInterrupts();
}

void setPWM3(unsigned int dutyCycle)
{
	// Duty_cycle in range [0,100]
	OC1RS = ((PR3+1)*dutyCycle)/100;
}


void putc(char byte2send)
{
	// wait while(UTXBF == 1)
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}

void puts(char *str)
{
	while(*str != '\0')
	{
		putc(*str);
		str++;
	}
}

void configUart(unsigned int baud, char parity, unsigned int stopbits)
{
	// 1 - Configure BaudRate Generator
	U1MODEbits.BRGH = 0;
	U1BRG = ((20000000 + 8*baud)/(16*baud)) - 1;
	// 2 - Configure # of data bits, parity and # of stopbits
	//     ( see U1MODE register)
	// PDSEL<1:0>: Parity and Data Selection bits
	// 11 = 9-bit data, no parity
	// 10 = 8-bit data, odd parity
	// 01 = 8-bit data, even parity
	// 00 = 8-bit data, no parity
	switch(parity)
	{
		case 'N':
			U1MODEbits.PDSEL = 00;
			break;
		case 'E':
			U1MODEbits.PDSEL = 01;
			break;
		case 'O':
			U1MODEbits.PDSEL = 10;
			break;
		default:
			break;
	}
	U1MODEbits.STSEL = stopbits - 1;
	// 3 - Enable the Tx and Rx modules ( see register U1STA)
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	// 4 - Enable UART1 ( see register U1MODE)
	U1MODEbits.ON = 1;	
	// Configure UART Interrupts
	// Configuração do TX e RX para
	U1STAbits.UTXSEL = 00;		// ativar posição livre
	// ESTA PARTE ESTÁ MAL IMPLEMENTADA E NECESSITA DE REVISÃO
	// O UTXSEL DEVIA SER UTXISEL
	U1STAbits.URXISEL = 00;		// ativar leitura nova
	// Configurar TX e RX para ativação/inativação
	IEC0bits.U1TXIE = 0;
	IEC0bits.U1RXIE = 1;
	// Configurar prioridade
	IPC6bits.U1IP = 3;
	// Configuar deteção de erros framming, overrun, parity
	IEC0bits.U1EIE = 1;
}

char getc(void)
{
	// If OERR == 1 then reset OERR
	if(U1STAbits.OERR == 1)
		U1STAbits.OERR = 0;
	// Wait while URXDA == 0
	//while(U1STAbits.URXDA == 0);
	// If FERR or PERR then
	// 	read UxRXREG (to discard the char) and return 0
	if(U1STAbits.FERR || U1STAbits.PERR)
	{
		int discard = U1RXREG;
		return 0;
	}
	// Else
	//	return U1RXREG
	else
		return U1RXREG;
}

int main(void)
{
	configureAll();
	unsigned int portVal;
	int dutyCycle;
	TRISEbits.TRISE4 = 1;
	TRISEbits.TRISE5 = 1;
	TRISEbits.TRISE6 = 1;
	TRISEbits.TRISE7 = 1;
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	OC1CONbits.ON = 1;
	configUart(115200,'N',1);
	EnableInterrupts();
	while(1)
	{
		portVal = (PORTB & 0x3);
		switch(portVal)
		{
			case 0:	// Measure input voltage
				// Enable T1 interrupts
				IEC0bits.T1IE = 1;	// Enable T1 interrupts
				IEC1bits.AD1IE = 1;
				setPWM3(0);	// LED off
				break;
			case 1:	// Freeze
				// Disable T1 interrupt
				IEC0bits.T1IE = 0;	// Disable T1 interrupts
				IEC1bits.AD1IE = 0;
				setPWM3(100);	// LED on
				break;
			default:// LED brigthness control
				IEC1bits.AD1IE = 1;
				// Enable T1 interrupts
				IEC0bits.T1IE = 1;
				dutyCycle = v*3;
				setPWM3(dutyCycle);
				break;
		}
	}
	return 0;
}

void _int_(4) isr_T1(void)
{
	AD1CON1bits.ASAM = 1;
	IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
	static int temp = 0;
	if(++temp == 100)
	{
		temp = 0;
		//itoa(v,str);
		//printf(str);
		//*str = ()+(toBcd(v)+0x30);
		//*str = (0xF0&(toBcd(v)+0x30)<<8) + (0xF&(toBcd(v))+0x30);
		//*str =(0xF&(toBcd(v))+0x30);
		//*str = ((toBcd(v)<< 1 + '0')<<1)+(toBcd(v)+'0');
		//puts(str);
		printInt10(v);
		putc('\n');
	}
	send2displays(toBcd(v));
	IFS0bits.T3IF = 0;
}

// Interrupt Handler
void _int_(27) isr_adc(void)	// Replace Vector by ADC
{					// vector number
	// Start ADC
	//AD1CON1bits.ASAM = 1;
	// Print ADC1BUF0 value in 0x and 3 digits
	// Read conversion result (ADC1BUF0 value)
	// print it
	v = 0;
	int *p = (int *)(&ADC1BUF0);
	//printStr("\r");
	for(; p <= (int *)(&ADC1BUF7); p+=4)
	{
		//printInt(*p, 10 | 4 << 16);
		//printStr(" "); 
		v += *p;
	}
	v = v/8;
	v = (v * 33 + 511)/1023;
	if(voltMin > v)
	{
		voltMin = v;
	}
	if(voltMax < v)
	{
		voltMax = v;
	}
	// Reset AD1IF
	IFS1bits.AD1IF = 0;
}

void _int_(24) isr_uart1(void)
{
	if(IFS0bits.U1EIF)
	{
		if(U1STAbits.OERR == 1)
		{
   			U1STAbits.OERR = 0;
   		}
   		else
   		{
   			int dump = U1RXREG;
   		}
   		IFS0bits.U1EIF = 0;
	}
	
	if(IFS0bits.U1RXIF == 1)
	{
		// Clear UART1 RX interrupt flag
		if(U1RXREG == 'L')
		{
			putc('\n');
			puts("voltMin = ");
			printInt10(voltMin);
			puts(" voltMax = ");
			printInt10(voltMax);
			putc('.');
			putc('\n');
		}
		IFS0bits.U1RXIF = 0;
	}
	if(IFS0bits.U1TXIF == 1)
	{
		
		IFS0bits.U1TXIF = 0;
	}
}
