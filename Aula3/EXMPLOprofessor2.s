task3:
	lui $t0, SFR_BASE_HI
	lw $t1, TRISB($t0)
	ori $t1, $t1, 0x000F
	
	lw $t1, TRISE($t0)
	andi $t1, $t1, 0xFFF0
	sw $t1, TRISE($t0)
	
loop:	lw $t1, PORTB($t0)
	andi $t1, $t1, 0x000F
	xori $t1, $t1, 0x0009
	
	lw $t2, LATE($t0)
	andi $t2, $t2, 0xFFF0
	ori $t2, $t2, $t1
	sw $t2, LATE($t0)
	
	j loop
