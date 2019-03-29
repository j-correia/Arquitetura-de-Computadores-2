# Exercícios 1, 2 e 3 condensados num único programa em Assembly que, através de menu em loop infinito, permite escolher o programa a executar
	.equ SFR_BASE_HI, 0xBF88	# 16 MSbits of SFR area
	.equ TRISE, 0x6100		# TRISE address is 0xBF886100
	.equ PORTE, 0x6110		# PORTE address is 0xF886110
	.equ LATE, 0x6120		# LATE address is 0xBF886120
