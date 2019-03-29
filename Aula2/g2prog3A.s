	.equ PRINT_INT, 6		# $a0 = val, $a1 = base
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11	
	.data
	.text
	.globl main
main:	subu $sp, $sp, 20	# reservar espaço na stack
	sw $ra, 0($sp)		# salvaguarda de contexto
	sw $s0, 4($sp)		# salvaguarda de contexto
	sw $s1, 8($sp)		# salvaguarda de contexto
	sw $s2, 12($sp)		# salvaguarda de contexto
	sw $s3, 16($sp)		# salvaguarda de contexto
while:	lui $t1, 0xBF88		# while(1);
	lw $t2, 0x6050($t1)
	andi $t2, $t2, 0x0F
	or $a0, $0, $t2		# printInt($t2, 2)
	li $a1, 0x00040002	# printInt($t2, 2)
	ori $v0, $0, PRINT_INT	# printInt($t2, 2)
	syscall			# printInt($t2, 2)
	ori $a0, $0, 2000	# delay(20000);
	jal delay		# delay(20000);	
	j while			#
ewhile:	lw $ra, 0($sp)		# reposição de contexto
	lw $s0, 4($sp)		# reposição de contexto
	lw $s1, 8($sp)		# reposição de contexto
	lw $s2, 12($sp)		# reposição de contexto
	lw $s3, 16($sp)		# reposição de contexto
	addiu $sp, $sp, 20	# recolocação do stack-pointer
	jr $ra
	
# void delay(int ms)
# {
# 	for(; ms > 0; ms--)
#	{
#		resetCoreTimer();
#		while(readCoreTimer() < K);
#	}
# }
#
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
