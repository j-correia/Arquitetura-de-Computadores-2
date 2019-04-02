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
alna:	.asciiz "\nAlínea A."
alnb:	.asciiz "\nAlínea B."
alnc:	.asciiz "\nAlínea C."
alnd:	.asciiz "\nAlínea D."
alne:	.asciiz "\nAlínea E."
alnf:	.asciiz "\nAlínea F."
alng:	.asciiz "\nAlínea G."
	.text
	.globl main
main:	subu $sp, $sp, 4
	sw $ra, 0($sp)
loop:	la $a0, msg2
	ori $v0, $0, printStr	
	syscall
	#
	la $a0, msg1
	ori $v0, $0, printStr
	syscall
	#
	ori $v0, $0, getChar
	syscall
	#
a:	bne $v0, 'a', A
	jal alA
	j loop
A:	bne $v0, 'A', b
	jal alA
	j loop
b:	bne $v0, 'b', B
	jal alB
	j loop
B:	bne $v0, 'B', C
	jal alB
	j loop
c:	bne $v0, 'c', C
	jal alC
	j loop
C:	bne $v0, 'C', d
	jal alC
	j loop
d:	bne $v0, 'd', D
	jal alD
	j loop
D:	bne $v0, 'D', e
	jal alD
	j loop
e:	bne $v0, 'e', E
	jal alE
	j loop
E:	bne $v0, 'E', f
	jal alE
	j loop
f:	bne $v0, 'f', F
	jal alF
	j loop
F:	bne $v0, 'F', g
	jal alF
	j loop
g:	bne $v0, 'g', G
	jal alG
	j loop
G:	bne $v0, 'G', loop
	jal alG
	j loop
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

alA:	la $a0, alnA
	ori $v0, $0, printStr
	syscall
	jr $ra
	#
alB:	la $a0, alnB
	ori $v0, $0, printStr
	syscall
	jr $ra
	#
alC:	la $a0, alnC
	ori $v0, $0, printStr
	syscall
	jr $ra
	#
alD:	la $a0, alnD
	ori $v0, $0, printStr
	syscall
	jr $ra
	#
alE:	la $a0, alnE
	ori $v0, $0, printStr
	syscall
	jr $ra
	#
alF:	la $a0, alnF
	ori $v0, $0, printStr
	syscall
	jr $ra
	#
alG:	la $a0, alnG
	ori $v0, $0, printStr
	syscall
	jr $ra
