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
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11	
	.data
	.text
	.globl main
main:	
delay:	ble $a0, 0, edelay
	ori $v0, $0, resetCoreTimer
	syscall
wdelay:	ori $v0, $0, readCoreTimer
	bge $v0, 20000, delay	
edelay: jr $ra
