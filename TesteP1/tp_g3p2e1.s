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
alnA:	.asciiz "\nAlínea A."
alnB:	.asciiz "\nAlínea B."
alnC:	.asciiz "\nAlínea C."
alnD:	.asciiz "\nAlínea D."
alnE:	.asciiz "\nAlínea E."
alnF:	.asciiz "\nAlínea F."
alnG:	.asciiz "\nAlínea G."
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
	lui $t1, SFR_BASE_HI
	ori $t2, $0, 0xFFF0		# RE0 -> 0utput
	sw $t2, TRISE($t1)		# RE0 -> 0utput
	ori $t2, $0, 0x000F		# RB0 -> 1nput
	sw $t2, TRISB($t1)		# RB0 -> 1nput
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
#funções intermédias
alA:	subu $sp, $sp, 4
	sw $ra, 0($sp)	
	la $a0, alnA
	ori $v0, $0, printStr
	syscall
stTA:	ori $s0, $0, 0
loopA:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', alAe		# if($v0 = 'm') goto exit1
	beq $v0, 'M', alAe		# if($v0 = 'M') goto exit1
	bge $s0, 16, stTA		# if($s0 => 16) $s0 = 0;
	ori $a0, $0, 500		# delay(500)
	jal delay			# delay(500)
	lui $t1, SFR_BASE_HI		# rd
	lw $t2, LATE($t1)		# rd
	andi $t2, $t2, 0xFFF0		# mf
	andi $s0, $s0, 0x000F		# mf
	or $t2, $t2, $s0		# mf
	sw $t2, LATE($t1)		# wr
	addu $s0, $s0, 1		# $s0++;
	j loopA				# goto loopA;
alAe:	lw $ra, 0($sp)
	addu $sp, $sp, 4
	jr $ra
	#
alB:	subu $sp, $sp, 4
	sw $ra, 0($sp)
	la $a0, alnB
	ori $v0, $0, printStr
	syscall
stTB:	ori $s0, $0, 15
loopB:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', alBe		# if($v0 = 'm') goto exit1
	beq $v0, 'M', alBe		# if($v0 = 'M') goto exit1
	blt $s0, 0, stTB		# if($s0 < 0) goto stTB
	ori $a0, $0, 125		# delay(250)
	jal delay
	lui $t1, SFR_BASE_HI		# rd
	lw $t2, LATE($t1)		# rd
	andi $t2, $t2, 0xFFF0		# mf
	andi $s0, $s0, 0x000F		# mf
	or $t2, $t2, $s0		# mf
	sw $t2, LATE($t1)		# wr
	subu $s0, $s0, 1		# $s0--;
	j loopB
alBe:	lw $ra, 0($sp)
	addu $sp, $sp, 4
	jr $ra
	#
alC:	la $a0, alnC
	ori $v0, $0, printStr
	syscall
	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', alCe		# if($v0 = 'm') goto exit1
	beq $v0, 'M', alCe		# if($v0 = 'M') goto exit1
alCe:	jr $ra
	#
alD:	la $a0, alnD
	ori $v0, $0, printStr
	syscall
	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', alDe		# if($v0 = 'm') goto exit1
	beq $v0, 'M', alDe		# if($v0 = 'M') goto exit1
alDe:	jr $ra
	#
alE:	la $a0, alnE
	ori $v0, $0, printStr
	syscall
	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', alEe		# if($v0 = 'm') goto exit1
	beq $v0, 'M', alEe		# if($v0 = 'M') goto exit1
alEe:	jr $ra
	#
alF:	la $a0, alnF
	ori $v0, $0, printStr
	syscall
	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', alFe		# if($v0 = 'm') goto exit1
	beq $v0, 'M', alFe		# if($v0 = 'M') goto exit1
alFe:	jr $ra
	#
alG:	la $a0, alnG
	ori $v0, $0, printStr
	syscall
	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', alGe		# if($v0 = 'm') goto exit1
	beq $v0, 'M', alGe		# if($v0 = 'M') goto exit1
alGe:	jr $ra
