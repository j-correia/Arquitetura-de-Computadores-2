#include <detpic32.h>
#include "i2c.h"
static int temperature, tMin, tMax, time = 0, tFlag = 0;

int getTemperature(int *temperature)
{
	int ack;
	// Send Start event
	i2c1_start();
	// Send Address + WR (ADDR_WR); copy return
	// value to "ack" variable
	ack = i2c1_send(ADDR_WR);
	// Send Command (RTR); add return value to 
	// "ack" variable
	ack += i2c1_send(RTR);
	// Send Start event (again)
	i2c1_start();
	// Send Address + RD (ADDR_RD); add return 
	// value to "ack" variable
	ack += i2c1_send(ADDR_RD);
	// Test "ack" variable; if "ack" != 0 then
	// an error has occurred; send the Stop event,
	// print an error message and exit loop
	if(ack != 0)
	{
		i2c1_stop();
		printStr("An error has occured.\n");
		exit(0);
	}
	// Receive a value from slave (send NACK as
	// argument); copy
	// received value to "temperature" variable
	*temperature = i2c1_receive(I2C_NACK);
	// Send Stop event
	i2c1_stop();
	return ack;
}

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

void configAll(void)
{
	EnableInterrupts();
	// Display Module Port Configuration
	LATB = (LATB & 0x00FF);
	TRISB = (TRISB & 0x00FF);
	LATD = (LATD & 0xFF9F);
	TRISD = (TRISB & 0xFF9F);
	// Timer 2 Configuration
	T2CONbits.TCKPS = 7;	// fout_presc = 78.125 KHz
	PR2 = 19530;
	TMR2 = 0;		// Reset timer T2 count register
	T2CONbits.TON = 1;	// Enable T2
	IPC2bits.T2IP = 2;	// Interrupt priority
	IEC0bits.T2IE = 1;	// Enable timer T2 interrupts
	IFS0bits.T2IF = 0;	// Reset timer T2 interrupt Flag
	// Timer 3 Configuration
	T3CONbits.TCKPS = 0;	// fout_presc = 1 KHz
	PR3 = (20000-1);
	TMR3 = 0;		// Reset Timer T3 count Register
	T3CONbits.TON = 1;	// Enable T3
	IPC3bits.T3IP = 2;	// Interrupt priority
	IEC0bits.T3IE = 1;	// Enable timer T3 Interrupts
	IFS0bits.T3IF = 0;	// Reset T3 interrupt Flag
	// Timer 4 Configuration
	T4CONbits.TCKPS = 3;	// Fout_presc = 45 Hz;
	PR4 = 55555;		//
	TMR4 = 0;		// Reset Timer T4 count Register
	T4CONbits.TON = 1;	// Enable Timer T4
	IPC4bits.T4IP = 2;	// Interrupt Priority
	IEC0bits.T4IE = 1;	// Enable timer T4 Interrupts
	IFS0bits.T4IF = 0;	// Reset T4 interrupt Flag	
	// Random ports
	TRISB = (TRISB & 0xFFFF) | 0x0002;
	LATE = (LATE & 0xFFF0);
	TRISE = TRISE & 0xFFF0;
}



void _int_(8) isr_T2(void)
{
	// Wait 250 ms
	// Print "temperature" variable (syscall printInt10)
	getTemperature(&temperature);
	if(time == 0)
	{
		tMin = temperature;
		tMax = temperature;
	}
	if(tMin >= temperature)
	{
		LATEbits.LATE2 = 1;
		LATEbits.LATE3 = 0;
	}
	else if(tMax <= temperature)
	{
		tMax = temperature;
		LATEbits.LATE3 = 1;	
	}
	else
	{
		LATEbits.LATE2 = 0;
		LATEbits.LATE3 = 0;	
	}
	//printInt10(temperature);
	//printStr("\r");
	static int i;
	while(i++ > 3)
	{
		printf("A temperatura decorridos %d segundos é de %d tendo sido registadas [tMin, tMax] = [%d, %d]\r", time, temperature, tMin, tMax);
		printf("\r");
		i = 0;
	}
	if(tFlag == 3)
	{
		time++;
		tFlag = 0;
	}
	else
		tFlag++;
	LATEbits.LATE1 = !LATEbits.LATE1;
	IFS0bits.T2IF = 0;
}

void _int_(12) isr_T3(void)
{
	if((PORTB & 0x0002) == 0x0002)
		send2displays(toBcd(temperature));
	IFS0bits.T3IF = 0;
}

void _int_(16) isr_T4(void)
{
	LATEbits.LATE0 = !LATEbits.LATE0;
	IFS0bits.T4IF = 0;	
}

int main(void)
{
	i2c1_init(TC74_CLK_FREQ);
	configAll();
	while(1);
	return 0;	
}
