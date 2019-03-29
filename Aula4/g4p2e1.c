void send2displays(unsigned char value)
{
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	unsigned char dh = value >> 4;
	dh = display7Scodes[dh];
	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 1;
	dh = dh << 8;
	LATB = (LATB & 0x80FF) + dh;
	unsigned char dl = value & 0x0F;
	dl = display7Scodes[dl];
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;
	dl = dl << 8;
	LATB = (LATB & 0x80FF) + dl;
}
