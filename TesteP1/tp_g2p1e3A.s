	.equ printInt, 6 	# $a0 = val, $a1 = base
	.equ readCoreTimer, 11
	.equ resetCoreTimer, 12
	.data
	.text
	.globl main
main:	subu $sp, $sp, 12
	sw $s0, 0($sp)
	sw $s1, 4($sp)
	sw $ra, 8($sp)
while:	lui $t1, 0xBF88
	lw $t2, 0x6050($t1)
	andi $t2, $t2, 0x000F
	or $a0, $0, $t2
	li $a1, 0x00040002
	ori $v0, $0, printInt
	syscall
	ori $a0, $0, 500
	jal delay
	j while
	lw $s0, 0($sp)
	lw $s1, 4($sp)
	lw $ra, 8($sp)
	addu $sp, $sp,12
	jr $ra
	
delay:	beq $a0, 0, edelay
	ori $v0, $0, resetCoreTimer
	syscall
wdelay:	ori $v0, $0, readCoreTimer
	syscall
	blt $v0, 20000, wdelay
	sub $a0, $a0, 1
	j delay
edelay:	jr $ra
