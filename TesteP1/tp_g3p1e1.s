	.data
	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.equ TRISB, 0x6040
	.equ PORTB, 0x6050
	.equ LATB, 0x6060
	.text
	.globl main
main:	lui $t1, SFR_BASE_HI	#
	lw $t2, TRISE($t1)	# READ
	andi $t2, $t2, 0xFFFE	# MODIFY
	sw $t2, TRISE($t1)	# WRITE
	lui $t1, SFR_BASE_HI	#
	lw $t2, TRISB($t1)	# READ
	andi $t2, $t2, 0xFFFF	# MODIFY
	ori $t2, $t2, 0x0001	# MODIFY
	sw $t2, TRISB($t1)	# WRITE
	lui $t1, SFR_BASE_HI	#
	lw $t2, PORTB($t1)	# READ
	andi $t2, $t2, 0x0001	# MODIFY
	sw $t2, LATE($t1)	# WRITE
	j main
	jr $ra
