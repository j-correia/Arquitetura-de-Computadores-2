# void delay(int ms)
# {
#	for(; ms > 0; ms--)
#	{
#		resetCoreTimer();
#		while(readCoreTimer() < K);
# 	}
# }
	.equ readCoreTimer, 11
	.equ resetCoreTimer, 12
	.data
	.text
	.globl main
main:	
	ori $a0, $0, 1000
delay:	beq $a0, 0, edelay
	ori $v0, $0, resetCoreTimer
	syscall
wdelay:	ori $v0, $0, readCoreTimer
	syscall
	blt $v0, 20000, wdelay
	sub $a0, $a0, 1
	j delay
edelay:	jr $ra
