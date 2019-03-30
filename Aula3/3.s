.equ SFR_BASE_HI, 0xBF88	# 16 MSbits of SFR area
	.equ TRISE, 0x6100		# TRISE address is
					# 0xBF886100
	.equ PORTE, 0x6110		# PORTE address is
					# 0xF886110
	.equ LATE, 0x6120		# LATE address is
					# 0xBF886120
	.equ TRISB, 0x6040		# TRISB address is
					# 0xBF886040
	.equ PORTB, 0x6050		# PORTB address is
					# 0xBF886050
	.equ LATB, 0x6060		# LATB address is
					# 0xBF886060
	.equ readInt10,	5
	.equ printStr, 8
	.equ inkey, 1
	.equ getChar, 2
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11	
	.data
str0:	.asciiz "Prima a tecla M para retornar ao Menu.\n"
str1:	.asciiz "\nQual das Tarefas pretende?\n"
prg1:	.asciiz "\nA) Contador binário crescente 4 bits @1Hz"
prg2:	.asciiz "\nB) Contador binário decrescente 4 bits @4Hz"
prg3:	.asciiz "\nC) Contador binário crescente/decrescente por RB3 @2Hz"
prg4:	.asciiz "\nD) Contador Johnson de 4 bits à esquerda @1.5Hz"
prg5:	.asciiz "\nE) Contador Johnson de 4 bits à direita @1.5Hz"
prg6:	.asciiz "\nF) Contador Johnson de 4 bits à direita/esquerda por RB2 @ 1.5Hz"
prg7:	.asciiz "\nG) Contador Anel de 4 bits à esquerda/direita por RB1 @3Hz"
strin:	.asciiz "\nSeleção Inválida\n"
	.text
	.globl main
main:	subu  $sp, $sp, 4		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	ori $v0, $0, printStr		# printStr(prg7)
	la $a0, prg7			# printStr(prg7)
	syscall				# printStr(prg7)
	lui $t1, SFR_BASE_HI
	ori $t2, $0, 0xFFF0		# RE0...3 -> 0utput
	sw $t2, TRISE($t1)		# RE0...3 -> 0utput
	ori $t2, $0, 0xFFFF		# RB0...3 -> 1nput
	sw $t2, TRISB($t1)		# RB0...3 -> 1nput
	ori $t5, $0, 7
rstG:	beq $t5, 0, rstGlo		# if(flag== 0) goto rstGlo
	ori $t0, $0, 0x00000001
	j trfGlf
rstGlo:	ori $t0, $0, 0x00000008
	j trfGrg
trfGlp:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', extrfG		# if($v0 = 'm') goto exit1
	beq $v0, 'M', extrfG		# if($v0 = 'M') goto exit1
	lui $t1, SFR_BASE_HI	
	lw $t7, PORTB($t1)		# Ler Porto B
	andi $t7, $t7, 0x0002 		# Selecionar RB1	
	sll $t7, $t7, 1			# Reposicionar $t7
	beq $t7, 1, trfGlf		# if(RB1 = 1) goto trfGlf
	beq $t7, 0, trfGrg		# if(RB1 = 0) goto trfGrg
	ori $a0, $0, 333
	la $t7, delay
	jalr $t7
trfGlf:	ori $t5, $0, 1			# flag = 1
	bgt $t0, 0x0008, rstG
	blt $t0, 0x0001, rstG
	lui $t1, SFR_BASE_HI		
	sw $t0, LATE($t1)		# Escreve $t2 no RE
	sll $t0, $t0, 1
	j trfGlp
trfGrg:	ori $t5, $0, 0			# flag = 0
	blt $t0, 0x0001, rstG
	bgt $t0, 0x0008, rstG
	lui $t1, SFR_BASE_HI		
	sw $t0, LATE($t1)		# Escreve $t2 no RE
	srl $t0, $t0, 1
	j trfGlp
extrfG:	lw $ra, 0($sp)			# Reposição de contexto
	addu $sp, $sp, 4		# Libertar stack
	jr $ra
# void delay(int ms);
# Sub-rotina terminal -- Sem Salvaguarda
# Entrada:	($a0)
# Saída:	(void)
#

delay:	beq $a0, 0, edelay
	ori $v0, $0, resetCoreTimer
	syscall
wdelay:	ori $v0, $0, readCoreTimer
	syscall
	blt $v0, 20000, wdelay
	sub $a0, $a0, 1
	j delay	
edelay: jr $ra
