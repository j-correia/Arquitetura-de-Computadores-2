# João Manuel Bola Correia, 76493, P1
# Exercícios 1, 2 e 3 condensados num único programa
# em Assembly que, através de menu em loop infinito, 
# permite escolher o programa a executar.
# Enquanto dentro de cada programa (1, 2, 3)
# se premida a tecla M ( m, M) volta-se ao menu
# principal aonde se pode escolher outro programa.
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
	.data
str0:	.asciiz "Prima a tecla M para retornar ao Menu.\n"
str1:	.asciiz "\nQual dos Exercicios pretende?\n"
prg1:	.asciiz "\nPrograma 1"
prg2:	.asciiz "\nPrograma 2"
prg3:	.asciiz "\nPrograma 3"
strin:	.asciiz "\nNumero Invalido\n"
	.text
	.globl main
main:	subu $sp, $sp, 20		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	sw $s0, 4($sp)			# Salvaguarda de contexto
	sw $s1, 8($sp)			# Salvaguarda de contexto
	sw $s2, 12($sp)			# Salvaguarda de contexto
	sw $s3, 16($sp)			# Salvaguarda de contexto
	la $a0, str0			# printStr(str0)
	ori $v0, $0, printStr		# printStr(str0)
	syscall
while:					# while(1){
	la $a0, str1			# printStr(str1)
	ori $v0, $0, printStr		# printStr(str1)
	syscall				# printStr(str1)
	ori $v0, $0, readInt10		# $v0 = readInt10()
	syscall 			# $v0 = readInt10()
	blt $v0, 1, inv			# if($v0 < 1) goto inv
	bgt $v0, 3, inv			# if($v0 > 3) goto inv
	bne $v0, 1, b2			# if($v0 != 1) goto b2
	jal prog1			# prog1();
	j while				# goto while
b2:	bne $v0, 2, b3			# if($v0 != 2) goto b3
	jal prog2			# prog2();
	j while				# goto while
b3:	jal prog3			# Só resta opção $v0 = 3
	j while				# goto  while	
inv:	la $a0, strin			# printStr(strin)
	ori $v0, $0, printStr		# printStr(strin)
	syscall				# printStr(strin)
	j while				# goto while	
	lw $s3, 16($sp)			# Reposição de contexto
	lw $s2, 12($sp)			# Reposição de contexto
	lw $s1, 8($sp)			# Reposição de contexto
	lw $s0, 4($sp)			# Reposição de contexto
	lw $ra, 0($sp)			# Reposição de contexto
	addu $sp, $sp, 20		# Recolocação do Stack-P
	jr $ra
	
# Sub-Rotina 1 terminal
# Não necessita de salvaguarda
# RE0 -> output
# RB0 -> input
# RE0 = RB0;
prog1:	la $a0, prg1			# printStr(prg1)
	ori $v0, $0, printStr		# printStr(prg1)
	syscall				# printStr(prg1)
	lui $t1, SFR_BASE_HI		# 
	ori $t2, $0, 0xFFFE		# RE0 -> 0utput
	sw $t2, TRISE($t1)		# RE0 -> 0utput
	ori $t2, $0, 0x0001		# RB0 -> 1nput
	sw $t2, TRISB($t1)		# RB0 -> 1nput
p1lp:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', exit1		# if($v0 = 'm') goto exit1
	beq $v0, 'M', exit1		# if($v0 = 'M') goto exit1
	lw $t0, PORTB($t1)		# ler RB0
	sw $t0, LATE($t1)		# Escrever $t0 em RE0
	j p1lp				# goto p1lp
exit1: 	jr $ra

# Sub-Rotina 2 terminal
# Não necessita de salvaguarda
# RE0 -> output
# RB0 -> input
# RE0 = RB0\;	
prog2: 	la $a0, prg2			# printStr(prg2)
	ori $v0, $0, printStr		# printStr(prg2)
	syscall				# printStr(prg2)
	lui $t1, SFR_BASE_HI		# 
	ori $t2, $0, 0xFFFE		# RE0 -> 0utput
	sw $t2, TRISE($t1)		# RE0 -> 0utput
	ori $t2, $0, 0x0001		# RB0 -> 1nput
	sw $t2, TRISB($t1)		# RB0 -> 1nput
p2lp:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', exit2		# if($v0 = 'm') goto exit1
	beq $v0, 'M', exit2		# if($v0 = 'M') goto exit1
	lw $t0, PORTB($t1)		# ler RB0
	not $t0, $t0			# $t0 = RB0\
	sw $t0, LATE($t1)		# escrever $t0 em RE0
	j p2lp				# goto p2lp
exit2:	jr $ra
	
# Sub-Rotina 3 terminal
# Não necessita de salvaguarda
# RE0 -> output
# RB0 -> input
# RE0 = RB0\;
# RE1 = RB1;
# RE2 = RB2;
# RE3 = RB3\;
prog3: 	la $a0, prg3			# printStr(prg3)
	ori $v0, $0, printStr		# printStr(prg3)
	syscall				# printStr(prg3)
	lui $t1, SFR_BASE_HI
	ori $t2, $0, 0xFFF0		# RE0 -> 0utput
	sw $t2, TRISE($t1)		# RE0 -> 0utput
	ori $t2, $0, 0x000F		# RB0 -> 1nput
	sw $t2, TRISB($t1)		# RB0 -> 1nput
p3lp:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', exit3		# if($v0 = 'm') goto exit3
	beq $v0, 'M', exit3		# if($v0 = 'M') goto exit3
	or $t2, $0, $0			# $t2 = 0
	# bit 0
	lw $t0, PORTB($t1)		# ler RB0...3	
	not $t0, $t0			# RB0...3\
	andi $t3, $t0, 0x0001		# Seleciona bit 0
	or $t2, $t2, $t3		# Guarda bit 0 em $t2
	# bit 1
	lw $t0, PORTB($t1)		# ler RB0...3
	andi $t3, $t0, 0x0002		# Seleciona bit 1
	or $t2, $t2, $t3		# Guarda bit 1 em $t2
	# bit 2
	lw $t0, PORTB($t1)		# ler RB0...3
	andi $t3, $t0, 0x0004		# Seleciona bit 2
	or $t2, $t2, $t3		# Guarda bit 2 em $t2
	# bit 3
	lw $t0, PORTB($t1)		# ler RB0...3
	not $t0, $t0			# RB0...3\
	andi $t3, $t0, 0x0008		# Seleciona bit 3
	or $t2, $t2, $t3		# Guarda bit 3 em $t2
	sw $t2, LATE($t1)		# Escreve $t2 em RE0...3
	j p3lp				# goto p3lp
exit3:	jr $ra
