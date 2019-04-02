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
main:	lui $t1, SFR_BASE_HI	# SET RE0..3 as INPUT
	lw $t2, TRISE($t1)	# READ
	andi $t2, $t2, 0xFFF0	# MODIFY
	sw $t2, TRISE($t1)	# WRITE
	lui $t1, SFR_BASE_HI	# SET RB0..3 as OUTPUT
	lw $t2, TRISB($t1)	# READ
	andi $t2, $t2, 0xFFFF	# MODIFY
	ori $t2, $t2, 0x000F	# MODIFY
	sw $t2, TRISB($t1)	# WRITE
loop:	lui $t1, SFR_BASE_HI	# Load RE
	lw $t2, PORTB($t1)	# READ
	# bit 0
	nor $t3, $t2, $t2
	andi $t3, $t3, 0x0001
	andi $t2, $t2, 0xFFFE
	or $t2, $t2, $t3
	# bit 1
	# não faço nada
	# bit 2
	# nada faço
	# bit 3
	nor $t3, $t2, $t2
	andi $t3, $t3, 0x0008
	andi $t2, $t2, 0xFFF7
	or $t2, $t2, $t3
	andi $t2, $t2, 0x000F	# MODIFY
	lui $t1, SFR_BASE_HI	#
	sw $t2, LATE($t1)	# WRITE
	j loop
	jr $ra
