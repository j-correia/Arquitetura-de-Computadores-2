	.equ PRINT_INT, 6		# $a0 = val, $a1 = base
	.data
	.text
	.globl main
main:	addiu $sp, $sp, 20	# reservar espaço na stack
	sw $ra, 0($sp)		# salvaguarda de contexto
	sw $s0, 4($sp)		# salvaguarda de contexto
	sw $s1, 8($sp)		# salvaguarda de contexto
	sw $s2, 12($sp)		# salvaguarda de contexto
	sw $s3, 16($sp)		# salvaguarda de contexto
while:	lui $1, 0xBF88		# while(1);
	lw $t2, 0x6050($t1)
	or $a0, $0, $t1		# printInt($t2, 10)
	ori $a1, $0, 10		# printInt($t2, 10)
	ori $v0, $0, PRINT_INT	# printInt($t2, 10)
	syscall			# printInt($t2, 10)
	ori $a0, $0, 2000	# delay(20000);
	j delay			# delay(20000);	
	j while			#
ewhile:	lw $ra, 0($sp)		# reposição de contexto
	lw $s0, 4($sp)		# reposição de contexto
	lw $s1, 8($sp)		# reposição de contexto
	lw $s2, 12($sp)		# reposição de contexto
	lw $s3, 16($sp)		# reposição de contexto
	subu $sp, $sp, 20	# recolocação do stack-pointer
	jr $ra
	
# void delay(int ms);
# Sub-rotina terminal -- Sem Salvaguarda
# Entrada:	($a0)
# Saída:	(void)
delay:	# Manter comentada a instrução seguinte	
	# ori $a0, $a0, 0		
	ori $t0, $0, 20000000000# $t0 = K
for:	ble $a0, $0, efor	# if($a0 <= $0) goto efor
	ori $v0, $0, 12		# resetCoreTimer();
	syscall			# resetCoreTimer();
while:	ori $v0, $0, 11		# readCoreTimer();
	syscall 		# readCoreTimer();
	blt $v0, $t0, while	# if( readCoreTimer() < K) goto while
	j for			# }
efor: 	jr $ra			# }
