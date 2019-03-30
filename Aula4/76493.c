// Joao Correia, 76493, P1
#include <detpic32.h>
void ex1f(void);
void ex2(void);
void delay(int ms);
int main(void)
{
	while(1)
	{
		printf("\nPremir a tecla 'M' fá-lo-á retornar ao menu de escolha.1\n");
		printf("\nSelecione entre '1' para o Exercício 1.f) e '2' para o Exercício 2.\n");
		char op = getChar();
		switch(op)
		{
			case '1':
				printf("\nExercício 1.f).\n");
				ex1f();
				break;
			case '2':
				printf("\nExercício 2.\n");
				ex2();
				break;
			default:
				printf("\nEscolha Inválida.\n");
		}
	}
	return 0;
}

void ex1f(void)
{
	LATE = (LATE & 0x0000);
	TRISE = (TRISE & 0xFFF0);
	TRISBbits.TRISB2 = 1;
	int cont = 0;
	int bit;
	while(1)
	{
		if((PORTB & 0x0002) == 2)
		{
			bit = cont & 0x8;
			bit = ~bit;
			bit = bit >> 3;
			bit = 0x0001 & bit;
			cont = 0x000F & ((cont << 1) + bit);
		}
		if((PORTB & 0x0002) == 0)
		{
			bit = cont & 0x1;
			bit = ~bit;
			bit = bit << 3;
			bit = 0x0008 & bit;
			cont = 0x000F & ((cont >> 1) + bit);
		}		
		LATE = (LATE & 0xFFF0) | (0x000F & cont);
		char c = inkey();
		if(c == 'M' || c == 'm')
			return;
		delay(667);
	}
}

void ex2(void)
{
	LATE = (LATE & 0x0000);
	TRISE = (TRISE & 0xFFF0);
	int cont = 0x0;
	while(1)
	{
		if(cont == 0x10)
		{
			cont = 0x0;
		}
		LATE = (LATE & 0xFFF0) | cont;
		cont++;
		char c = inkey();
		if(c == 'M' || c == 'm')
			return;
		delay(250);
	}
}

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);	
	}

}
