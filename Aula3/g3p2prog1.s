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
main:	subu  $sp, $sp, 20		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	sw $s0, 4($sp)			# Salvaguarda de contexto
	sw $s1, 8($sp)			# Salvaguarda de contexto
	sw $s2, 12($sp)			# Salvaguarda de contexto
	sw $s3, 16($sp)			# Salvaguarda de contexto
while:	ori $v0, $0, printStr		# printStr(str0)
	la $a0, str0			# printStr(str0)
	syscall				# printStr(str0)
	ori $v0, $0, printStr		# printStr(str1)
	la $a0, str1			# printStr(str1)
	syscall				# printStr(str1)
	ori $v0, $0, printStr		# printStr(prg1)
	la $a0, prg1			# printStr(prg1)
	syscall				# printStr(prg1)
	ori $v0, $0, printStr		# printStr(prg2)
	la $a0, prg2			# printStr(prg2)
	syscall				# printStr(prg2)
	ori $v0, $0, printStr		# printStr(prg3)
	la $a0, prg3			# printStr(prg3)
	syscall				# printStr(prg3)
	ori $v0, $0, printStr		# printStr(prg4)
	la $a0, prg4			# printStr(prg4)
	syscall				# printStr(prg4)
	ori $v0, $0, printStr		# printStr(prg5)
	la $a0, prg5			# printStr(prg5)
	syscall				# printStr(prg5)
	ori $v0, $0, printStr		# printStr(prg6)
	la $a0, prg6			# printStr(prg6)
	syscall				# printStr(prg6)
	ori $v0, $0, printStr		# printStr(prg7)
	la $a0, prg7			# printStr(prg7)
	syscall				# printStr(prg7)
	ori $v0, $0, getChar		# $v0 = getChar();
	syscall				# $v0 = getChar();
	blt $v0, 'A', inv		#
	bgt $v0, 'G', maisc		#
maisc:	blt $v0, 'a', inv		#
	bgt $v0, 'g', inv		#
	bne $v0, 'A', b1		#
	jal trfA			#
	j while				#
b1:	bne $v0, 'a', b2		#
	jal trfA			#
	j while				#
b2:	bne $v0, 'B', b2a		#
	jal trfB			#
	j while				#
b2a:	bne $v0, 'b', b3		#
	jal trfB			#
	j while				#
b3:	bne $v0, 'C', b3a		#
	jal trfB			#
	j while				#
b3a:	bne $v0, 'c', b4		#
	jal trfC			#
	j while				#
b4:	bne $v0, 'D', b4a		#
	jal trfD			#
	j while				#
b4a:	bne $v0, 'd', b5		#
	jal trfD			#
	j while				#
b5:	bne $v0, 'E', b5a		#
	jal trfE			#
	j while				#
b5a:	bne $v0, 'e', b6		#
	jal trfE			#
	j while				#
b6:	bne $v0, 'F', b6a		#
	jal trfF			#
	j while				#
b6a:	bne $v0, 'f', b7		#
	jal trfF			#
	j while				#
b7:	bne $v0, 'G', b7a		#
	jal trfG			#
	j while				#
b7a:	bne $v0, 'g', inv		#
	jal trfG			#
	j while				#	
inv:	la $a0, strin			# printStr(strin)
	ori $v0, $0, printStr		# printStr(strin)
	syscall				# printStr(strin)
	j while				# goto while	
	lw $ra, 0($sp)			# Reposição de contexto
	lw $s0, 4($sp)			# Reposição de contexto
	lw $s1, 8($sp)			# Reposição de contexto
	lw $s2, 12($sp)			# Reposição de contexto
	lw $s3, 16($sp)			# Reposição de contexto
	addu $sp, $sp, 20		# Libertar stack
	jr $ra
#
# Tarefa A
# Contador binário crescente de 4 bits, 
# atualizado com uma frequência de 1Hz.
# Sub-rotina intermédia -- Com Salvaguarda
# Entrada:	(void)
# Saída:	(void)
trfA:	subu  $sp, $sp, 4		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	ori $v0, $0, printStr		# printStr(prg1)
	la $a0, prg1			# printStr(prg1)
	syscall				# printStr(prg1)
	lui $t1, SFR_BASE_HI
	ori $t2, $0, 0xFFF0		# RE0...3 -> 0utput
	sw $t2, TRISE($t1)		# RE0...3 -> 0utput
rstA:	ori $t0, $0, 0			# int $t0 = 0;
trfAlp:	beq $t0, 16, rstA		# if($t0 = 15) goto rstA
	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', extrfA		# if($v0 = 'm') goto exit1
	beq $v0, 'M', extrfA		# if($v0 = 'M') goto exit1
	lui $t1, SFR_BASE_HI
	andi $t3, $t0, 0x000F		# Seleciona bits
	lw $t2, LATE($t1)		# Lê estado do port RE
	andi $t2, $t2, 0xFFF0		# Descarta Bits usados 
	or $t2, $t2, $t3		# Guarda bits em $t2
	sw $t2, LATE($t1)		# Escreve $t2 em RE0...3
	ori $a0, $0, 1000		# delay(1000);
	jal delay			# delay(1000);
	addi $t0, $t0, 1		# $t0 ++;
	j trfAlp
extrfA:	lw $ra, 0($sp)			# Reposição de contexto
	addu $sp, $sp, 4		# Libertar stack
	jr $ra
#
# Tarefa B
# Contador binário decrescente de 4 bits,
# atualizado com uma frequência de 4Hz.
# Sub-rotina intermédia -- Com Salvaguarda
# Entrada:	(void)
# Saída:	(void)
trfB:	subu  $sp, $sp, 4		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	ori $v0, $0, printStr		# printStr(prg2)
	la $a0, prg2			# printStr(prg2)
	syscall				# printStr(prg2)
	lui $t1, SFR_BASE_HI
	ori $t2, $0, 0xFFF0		# RE0...3 -> 0utput
	sw $t2, TRISE($t1)		# RE0...3 -> 0utput
rstB:	ori $t0, $0, 15			# int $t0 = 15	
trfBlp:	blt $t0, 0, rstB		# if($t0 < 0) goto rstB	
	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', extrfB		# if($v0 = 'm') goto exit1
	beq $v0, 'M', extrfB		# if($v0 = 'M') goto exit1
	lui $t1, SFR_BASE_HI
	andi $t3, $t0, 0x000F		# Seleciona bits
	lw $t2, LATE($t1)		# Lê estado do port RE
	andi $t2, $t2, 0xFFF0		# Descarta Bits usados 
	or $t2, $t2, $t3		# Guarda bits em $t2
	sw $t2, LATE($t1)		# Escreve $t2 em RE0...3
	ori $a0, $0, 250		# delay(250);
	jal delay			# delay(250);
	subu $t0, $t0, 1		# $t0--;
	j trfBlp
extrfB:	lw $ra, 0($sp)			# Reposição de contexto
	addu $sp, $sp, 4		# Libertar stack
	jr $ra
# 
# Tarefa C
# Sub-rotina intermédia -- Com Salvaguarda
# Entrada:	(void)
# Saída:	(void)
trfC:	subu  $sp, $sp, 4		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	ori $v0, $0, printStr		# printStr(prg3)
	la $a0, prg3			# printStr(prg3)
	syscall				# printStr(prg3)
	lui $t1, SFR_BASE_HI
	ori $t2, $0, 0xFFF0		# RE0...3 -> 0utput
	sw $t2, TRISE($t1)		# RE0...3 -> 0utput
	ori $t2, $0, 0xFFFF		# RB0...3 -> 1nput
	sw $t2, TRISB($t1)		# RB0...3 -> 1nput
rstC:	beq $t5, 1, rstCHI		# if($t5 = 1) goto rstCHI
	ori $t0, $0, 15			# int $t0 = 15
	j trfClf			# flag acusa left
rstCHI:	ori $t0, $0, 0			# int $t0 = 0;	
trfClp:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', extrfC		# if($v0 = 'm') goto exit1
	beq $v0, 'M', extrfC		# if($v0 = 'M') goto exit1
	lui $t1, SFR_BASE_HI		
	lw $t7, PORTB($t1)		# Ler porto B
	andi $t7, $t7, 0x0008		# Seleciona RB3
	srl $t7, $t7, 3			# Reposiciona $t7
	beq $t7, 1, trfCrg		# if($t7 == 1) goto trfCrg
	beq $t7, 0, trfClf		# if($t7 == 0) goto trfClf
trfCrg:	ori $t5, $0, 1			# flag = 1;
	bgt $t0, 15, rstC		# if( $t0 > 15) goto rstC
	lui $t1, SFR_BASE_HI
	andi $t3, $t0, 0x000F		# Seleciona bits
	lw $t2, LATE($t1)		# Lê estado do port RE
	andi $t2, $t2, 0xFFF0		# Descarta Bits usados 
	or $t2, $t2, $t3		# Guarda bits em $t2
	sw $t2, LATE($t1)		# Escreve $t2 em RE0...3
	ori $a0, $0, 500		# delay(500);
	jal delay			# delay(500);
	addi $t0, $t0, 1		# $t0++;
	j trfClp
trfClf:	ori $t5, $0, 0			# flag = 0;
	blt $t0, 0, rstC
	lui $t1, SFR_BASE_HI
	andi $t3, $t0, 0x000F		# Seleciona bits
	lw $t2, LATE($t1)		# Lê estado do port RE
	andi $t2, $t2, 0xFFF0		# Descarta Bits usados 
	or $t2, $t2, $t3		# Guarda bits em $t2
	sw $t2, LATE($t1)		# Escreve $t2 em RE0...3
	ori $a0, $0, 500		# delay(500);
	jal delay			# delay(500);
	subu $t0, $t0, 1		# $t0--;
	j trfClp
extrfC:	lw $ra, 0($sp)			# Reposição de contexto
	addu $sp, $sp, 4		# Libertar stack
	jr $ra
# 
# Tarefa D
# Contador Johnson de 4 bits,
# com uma frequência de atualização de 1.5 Hz;
# Sub-rotina intermédia -- Com Salvaguarda
# Entrada:	(void)
# Saída:	(void)
trfD:	subu  $sp, $sp, 4		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	ori $v0, $0, printStr		# printStr(prg4)
	la $a0, prg4			# printStr(prg4)
	syscall				# printStr(prg4)
	lui $t1, SFR_BASE_HI
	ori $t2, $0, 0xFFF0		# RE0...3 -> 0utput
	sw $t2, TRISE($t1)		# RE0...3 -> 0utput
	ori $t2, $0, 0			# int $t0 = 0;
trfDlp:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', extrfD		# if($v0 = 'm') goto exit1
	beq $v0, 'M', extrfD		# if($v0 = 'M') goto exit1
	ori $a0, $0, 667		# delay(667)
	jal delay			# delay(667)
	or $t3, $0, $t2			# selecionar bit 3
	not $t3, $t3			# selecionar bit 3
	andi $t3, $t3, 0x0008		# selecionar bit 3
	srl $t3, $t3, 3			# reposicionar bit 3 -> 0
	sll $t2, $t2, 1			# shift 1 bit
	or $t2, $t2, $t3		# Concatena
	lui $t1, SFR_BASE_HI		
	sw $t2, LATE($t1)		# Escreve $t2 no RE
	j trfDlp
extrfD:	lw $ra, 0($sp)			# Reposição de contexto
	addu $sp, $sp, 4		# Libertar stack
	jr $ra
#
# Tarefa E
# Contador Johnson de 4 bits com deslocamento à direita,
# frequência de atualização de 1.5 Hz
# Sub-rotina intermédia -- Com Salvaguarda
# Entrada:	(void)
# Saída:	(void)
trfE:	subu  $sp, $sp, 4		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	ori $v0, $0, printStr		# printStr(prg5)
	la $a0, prg5			# printStr(prg5)
	syscall				# printStr(prg5)
	lui $t1, SFR_BASE_HI
	ori $t2, $0, 0xFFF0		# RE0...3 -> 0utput
	sw $t2, TRISE($t1)		# RE0...3 -> 0utput
	ori $t2, $0, 0			# int $t0 = 0;
trfElp:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', extrfE		# if($v0 = 'm') goto exit1
	beq $v0, 'M', extrfE		# if($v0 = 'M') goto exit1
	ori $a0, $0, 667		# delay(667)
	jal delay			# delay(667)
	or $t3, $0, $t2			# selecionar bit 3
	not $t3, $t3			# selecionar bit 3
	andi $t3, $t3, 0x0001		# selecionar bit 3
	sll $t3, $t3, 3			# Recolocar bit 0 -> 3
	srl $t2, $t2, 1			# shift 1 bit
	or $t2, $t2, $t3		# Concatena
	lui $t1, SFR_BASE_HI		
	sw $t2, LATE($t1)		# Escreve $t2 no RE
	j trfElp
extrfE:	lw $ra, 0($sp)			# Reposição de contexto
	addu $sp, $sp, 4		# Libertar stack
	jr $ra
#
# Tarefa F
# Contador Johnson de 4 bits com deslocamento à esquerda
# ou à direita, dependendo do valor lido do porto de entrada RB2:
# se RB2=1, deslocamento à esquerda; 
# frequência deatualização de 1.5 Hz.
# Sub-rotina intermédia -- Com Salvaguarda
# Entrada:	(void)
# Saída:	(void)
trfF:	subu  $sp, $sp, 4		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	ori $v0, $0, printStr		# printStr(prg6)
	la $a0, prg6			# printStr(prg6)
	syscall				# printStr(prg6)
	lui $t1, SFR_BASE_HI
	ori $t2, $0, 0xFFF0		# RE0...3 -> 0utput
	sw $t2, TRISE($t1)		# RE0...3 -> 0utput
	ori $t2, $0, 0xFFFF		# RB0...3 -> 1nput
	sw $t2, TRISB($t1)		# RB0...3 -> 1nput
	ori $t2, $0, 0			# int $t0 = 0;
trfFlp:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', extrfF		# if($v0 = 'm') goto exit1
	beq $v0, 'M', extrfF		# if($v0 = 'M') goto exit1
	lui $t1, SFR_BASE_HI	
	lw $t7, PORTB($t1)		# Ler Porto B
	andi $t7, $t7, 0x0004 		# Selecionar RB2	
	srl $t7, $t7, 2			# Reposicionar $t7
	beq $t7, 1, trfFlf		# if(RB2 = 1) goto trfFlf
	beq $t7, 0, trfFrg		# if(RB2 = 0) goto trfFrg
trfFlf: ori $a0, $0, 667		# delay(667)
	jal delay			# delay(667)
	or $t3, $0, $t2			# selecionar bit 3
	not $t3, $t3			# selecionar bit 3
	andi $t3, $t3, 0x0008		# selecionar bit 3
	srl $t3, $t3, 3			# reposicionar bit 3 -> 0
	sll $t2, $t2, 1			# shift 1 bit
	or $t2, $t2, $t3		# Concatena
	lui $t1, SFR_BASE_HI		
	sw $t2, LATE($t1)		# Escreve $t2 no RE
	j trfFlp
trfFrg: ori $a0, $0, 667		# delay(667)
	jal delay			# delay(667)
	or $t3, $0, $t2			# selecionar bit 3
	not $t3, $t3			# selecionar bit 3
	andi $t3, $t3, 0x0001		# selecionar bit 3
	sll $t3, $t3, 3			# Recolocar bit 0 -> 3
	srl $t2, $t2, 1			# shift 1 bit
	or $t2, $t2, $t3		# Concatena
	lui $t1, SFR_BASE_HI		
	sw $t2, LATE($t1)		# Escreve $t2 no RE
	j trfFlp
extrfF:	lw $ra, 0($sp)			# Reposição de contexto
	addu $sp, $sp, 4		# Libertar stack
	jr $ra
#
# Tarefa G
# Contador em anel de 4 bits (ring counter) com deslocamento 
# à esquerda ou à direita, dependendo do valor lido do porto RB1:
# se RB1=1, deslocamento à esquerda.
# Frequência de atualização de 3 Hz
# Sub-rotina intermédia -- Com Salvaguarda
# Entrada:	(void)
# Saída:	(void)
trfG:	subu  $sp, $sp, 4		# Criar espaço na stack
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
	ori $t0, $0, 0x0001
	j trfGlf
rstGlo:	ori $t0, $0, 0x0008
	j trfGrg
trfGlp:	ori $v0, $0, inkey		# $v0 = inkey()
	syscall				# $v0 = inkey()
	beq $v0, 'm', extrfG		# if($v0 = 'm') goto exit1
	beq $v0, 'M', extrfG		# if($v0 = 'M') goto exit1
	ori $a0, $0, 333		# delay(333)
	la $t7, delay			# delay(333)
	jalr $t7			# delay(333)
	lui $t1, SFR_BASE_HI	
	lw $t7, PORTB($t1)		# Ler Porto B
	andi $t7, $t7, 0x0002 		# Selecionar RB1	
	sll $t7, $t7, 1			# Reposicionar $t7
	beq $t7, 1, trfGlf		# if(RB1 = 1) goto trfGlf
	beq $t7, 0, trfGrg		# if(RB1 = 0) goto trfGrg
trfGlf:	ori $t5, $0, 1			# flag = 1
	bgt $t0, 0x0008, rstG		# reset circle
	lui $t1, SFR_BASE_HI		
	sw $t0, LATE($t1)		# Escreve $t2 no RE
	sll $t0, $t0, 1			# shift circle
	j trfGlp
trfGrg:	ori $t5, $0, 0			# flag = 0
	blt $t0, 0x0001, rstG		# reset circle
	lui $t1, SFR_BASE_HI		
	sw $t0, LATE($t1)		# Escreve $t2 no RE
	srl $t0, $t0, 1			# shift circle
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
