#include <detpic32.h>

void delay(int ms);
int main(void)
{
	TRISE = (TRISE & 0xFFF0);
	int cont = 0;
	while(1)
	{
		if(cont ==0x10)
		{
			cont = 0x0;
		}
		LATE = (LATE & 0xFFF0) | cont;
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
