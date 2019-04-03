#import <detpic32.h>
void delay(int ms);
int main(void)
{
	int cont = 0;
	LATE = LATE & 0xFFF0;
	TRISE = TRISE & 0xFFF0;
	while(1)
	{
		if(cont == 16)
			cont = 0;
		LATE = (LATE & 0xFFF0) | (cont & 0x000F);
		cont++;
		delay(250);
	}
	return 0;
}

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);
	}
}
