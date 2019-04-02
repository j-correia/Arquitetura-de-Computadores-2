	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.equ TRISB, 0x6040
	.equ PORTB, 0x6050
	.equ LATB, 0x6060
	.equ inkey, 1
	.equ getChar, 2
	.equ printStr, 8
	.equ readCoreTimer, 11
	.equ resetCoreTimer, 12	
	.data
msg1:	.asciiz "\nQual das Alíneas: "
msg2:	.asciiz "\nPremir a tecla M volta ao menu."
	.text
	.globl main
main:	subu $sp, $sp, 4
	sw $ra, 0($sp)
	
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
