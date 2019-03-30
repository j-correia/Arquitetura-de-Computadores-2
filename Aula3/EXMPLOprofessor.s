	.data
	.text
	.globl main
main:	$t0, SFR_BASE_HI
	lw $t1, TRISB($t0)
	ori $t1, $t1, 0x0001
	sw $t1, TRISE($t0)
	lw $t1, TRISE($t0)
	andi $t1, $t1, 0xFFFE
	sw $t1, TRISE($t0)
loop:	lw $t1, PORTB($t0)
	andi $t1, $t1, 0x0001
	lw $t2, LATE($t0)
	andi $t2, $t2, 0xFFF0
	or $t2, $t2, $1
	sw $t2, LATE($t0)
	j loop
