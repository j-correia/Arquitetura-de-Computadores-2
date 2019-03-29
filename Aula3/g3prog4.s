	.equ SFR_BASE_HI, 0xBF88	# 16 MSbits of SFR area
	.equ TRISE, 0x6100		# TRISE address is
					# 0xBF886100
	.equ PORTE, 0x6110		# PORTE address is
					# 0xF886110
	.equ LATE, 0x6120		# LATE address is
					# 0xBF886120
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11	
	.data
	.text
	.globl main
main:	subu  $sp, $sp, 20		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	sw $s0, 4($sp)			# Salvaguarda de contexto
	sw $s1, 8($sp)			# Salvaguarda de contexto
	sw $s2, 12($sp)			# Salvaguarda de contexto
	sw $s3, 16($sp)			# Salvaguarda de contexto
	ori $t0, $0, 0			# int v = 0;
	lui $t1, SFR_BASE_HI	# 16MSbits do SFR
	lw $t2, TRISE($t1)		# Ler estado dos portos RE
	andi $t2, $t2, 0xFFFE		# Mudar bit RE0 para 0utput
	sw $t2, TRISE($t1)		# Escrever porto RE
while:	lui $t1, SFR_BASE_HI	# 16MSbits do SFR
	lw $t2, LATE($t1)		# Ler estado do porto RE
	andi $t0, $t0, 0xFFFE		# Selecionar bit de v
	and $t2, $t2, $t0		# Mudar bit RE0 -> v
	sw $t2, LATE($t1)		# Escrever porto RE
	ori $a0, $0, 500		# delay(500);
	jal delay			# delay(500);
	not $t0, $t0			# v ^= 1;
	j while				# goto while
	lw $ra, 0($sp)			# Reposição de contexto
	lw $s0, 4($sp)			# Reposição de contexto
	lw $s1, 8($sp)			# Reposição de contexto
	lw $s2, 12($sp)			# Reposição de contexto
	lw $s3, 16($sp)			# Reposição de contexto
	addu $sp, $sp, 20		# Libertar stack
	jr $ra
# void delay(int ms);
# Sub-rotina terminal -- Sem Salvaguarda
# Entrada:	($a0)
# Saída:	(void)
#
delay:	ble $a0, 0, edelay
	ori $v0, $0, resetCoreTimer
	syscall
wdelay:	ori $v0, $0, readCoreTimer
	bge $v0, 20000, delay	
edelay: jr $ra
