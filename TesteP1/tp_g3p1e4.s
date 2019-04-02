	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.equ TRISB, 0x6040
	.equ PORTB, 0x6050
	.equ LATB, 0x6060
	.equ readCoreTimer, 11
	.equ resetCoreTimer, 12	
	.data
	.text
	.globl main
main:	subu $sp, $sp, 4
	sw $ra, 0($sp)
	lui $t1, SFR_BASE_HI	# READ
	lw $t2, TRISE($t1)	# READ
	andi $t2, $t2, 0xFFFE	# MODIFY
	sw $t2, TRISE($t1)	# WRITE
	or $t0, $0, $0		# $t0 = 0
while:	lui $t1, SFR_BASE_HI	# READ
	lw $t2, LATE($t1)	# READ
	andi $t0, $t0, 0x0001	# MODIFY
	andi $t2, $t2, 0xFFFE	# MODIFY
	or $t2, $t2, $t0	# MODIFY
	lui $t1, SFR_BASE_HI	# WRITE
	sw $t2, LATE($t1)	# WRITE
	ori $a0, $0, 500	# delay(500)
	jal delay		# delay(500)
	xor $t0, $t0, 0x0001	# v ^= 1;
	j while
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	jr $ra
# delay
delay:	beq $a0, 0, edelay
	ori $v0, $0, resetCoreTimer
	syscall
wdelay:	ori $v0, $0, readCoreTimer
	syscall
	blt $v0, 20000, wdelay
	sub $a0, $a0, 1
	j delay	
edelay: jr $ra
