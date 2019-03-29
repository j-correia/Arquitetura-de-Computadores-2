// Joao Correia, 76493, P1
#include <detpic32.h>
void printDisplay(int number);
void delay(int ms);
int main(void)
{
	LATB = (LATB & 0x00FF);			// colocar só os bits que queremos a 0 e diversas inicializações
	TRISB = (TRISB & 0x00FF);
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 0;
	TRISBbits.TRISB0 = 1;
	TRISBbits.TRISB1 = 1;
	LATEbits.LATE0 = 0;
	TRISEbits.TRISE0 = 0;
	int ss = 0, mm = 0, hh = 0, time = 0, ledTemp = 0, mode = 0;
	LATEbits.LATE0 = 1;
	while(1)
	{
		if(time == 100)
		{
			ss++;
			time = 0;
		}
		if(ss == 60)
		{
			mm++;
			ss = 0;
		}
		if(mm == 60)
		{
			hh++;
			mm = 0;
		}
		if(hh == 24)
			hh = 0;
		if(ledTemp == 50)
		{
			ledTemp = 0;
			LATEbits.LATE0 = !LATEbits.LATE0;
		}
		if(PORTBbits.RB0 == 0 && PORTBbits.RB1 == 0)
			printDisplay(ss);	
		if(PORTBbits.RB0 == 0 && PORTBbits.RB1 == 1)
			printDisplay(hh);
		if(PORTBbits.RB0 == 1 && PORTBbits.RB1 == 0)
			printDisplay(mm);
		if(PORTBbits.RB0 == 1 && PORTBbits.RB1 == 1)
		{
			if(mode >= 0 && mode < 50 )
				printDisplay(ss);
			if(mode >= 50 && mode < 100)
				printDisplay(mm);
			if(mode >= 100 && mode <= 150)
				printDisplay(hh);
			mode++;
		}
		if(mode >= 150)
			mode = 0;
		printInt(hh,0x0002000A); // "base": 16 lsbits [2...16] -> 0x000A, "format": 16 msbits [#digitos que imprime] -> 0x0002
		printStr(":");
		printInt(mm,0x0002000A); // "base": 16 lsbits [2...16] -> 0x000A, "format": 16 msbits [#digitos que imprime] -> 0x0002
		printStr(":");
		printInt(ss,0x0002000A); // "base": 16 lsbits [2...16] -> 0x000A, "format": 16 msbits [#digitos que imprime] -> 0x0002
		printStr("\r");
		ledTemp++;
		time++;
	}	
	return 0;
}

void printDisplay(int number)		// sem assert, só para inteiros [0...99]
{
	int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	int digit1 = number % 10;
	number = number / 10;
	int digit2 = number % 10;
	LATDbits.LATD5 = 0;			// Selecionar disp2 
	LATDbits.LATD6 = 1;
	digit2 = display7Scodes[digit2] << 8;
	LATB = (LATB & 0x00FF) + digit2;
	delay(5); 				// DELAY NECESSÁRIO A MOSTRAR OS DIGITOS
	LATDbits.LATD5 = 1;			// Selecionar disp1 
	LATDbits.LATD6 = 0;
	digit1 = display7Scodes[digit1] << 8;
	LATB = (LATB & 0x00FF) + digit1;
	delay(5);				// Soma dos delay(5) permite frequência pedida
}

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);	
	}
}
