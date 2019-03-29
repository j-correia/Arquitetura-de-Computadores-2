# Exercícios 1, 2 e 3 condensados num único programa em Assembly que, através de menu em loop infinito, permite escolher o programa a executar
# João Manuel Bola Correia, 76493, P1

	.equ SFR_BASE_HI, 0xBF88	# 16 MSbits of SFR area
	.equ TRISE, 0x6100		# TRISE address is 0xBF886100
	.equ PORTE, 0x6110		# PORTE address is 0xF886110
	.equ LATE, 0x6120		# LATE address is 0xBF886120
	.data
	.text
	.globl main
main:	subu $sp, $sp, 20		# Criar espaço na stack
	sw $ra, 0($sp)			# Salvaguarda de contexto
	sw $s0, 4($sp)			# Salvaguarda de contexto
	sw $s1, 8($sp)			# Salvaguarda de contexto
	sw $s2, 12($sp)			# Salvaguarda de contexto
	sw $s3, 16($sp)			# Salvaguarda de contexto
while:					# while(1){
	
	j while				# }	
	lw $s3, 16($sp)			# Reposição de contexto
	lw $s2, 12($sp)			# Reposição de contexto
	lw $s1, 8($sp)			# Reposição de contexto
	lw $s0, 4($sp)			# Reposição de contexto
	lw $ra, 0($sp)			# Reposição de contexto
	addu $sp, $sp, 20		# Recolocação do Stack-P
	jr $ra
