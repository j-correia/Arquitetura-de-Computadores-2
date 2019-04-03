#import <detpic32.h>
int main(void)
{
	LATB = LATB & 0x00FF;
	TRISB = TRISB & 0x00FF;
	TRISD = TRISD & 0xFF9F;
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;
	while(1)
	{
		int c = getChar();
		LATB = LATB & 0x00FF;
		switch(c)
		{
			case 'a':
				LATBbits.LATB8 = 1;		
				break;
			case 'A':
				LATBbits.LATB8 = 1;
				break;
			case 'b':
				LATBbits.LATB9 = 1;
				break;
			case 'B':
				LATBbits.LATB9 = 1;
				break;
			case 'c':
				LATBbits.LATB10 = 1;
				break;
			case 'C':
				LATBbits.LATB10 = 1;
				break;
			case 'd':
				LATBbits.LATB11 = 1;
				break;
			case 'D':
				LATBbits.LATB11 = 1;
				break;
			case 'e':
				LATBbits.LATB12 = 1;
				break;
			case 'E':
				LATBbits.LATB12 = 1;
				break;
			case 'f':
				LATBbits.LATB13 = 1;
				break;
			case 'F':
				LATBbits.LATB13 = 1;
				break;
			case 'g':
				LATBbits.LATB14 = 1;
				break;
			case 'G':
				LATBbits.LATB14 = 1;
				break;
			default:
				LATBbits.LATB15 = 1;
				break;
		}
	}
	return 0;
}
