# void main(void)
# {
#	int counter = 0;
#	while(1)
#	{
#		while(readCoreTimer() < 200000);
#		resetCoreTimer();
#		printInt(++counter, 10);
#		putChar(' ');			//space
#	}
# }
	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.equ putChar, 3
	.equ printInt, 6
	.data
	.text
	.globl main
main:	ori $t0, $0, 0
while:	ori $v0, $0, READ_CORE_TIMER	# while(1){
	syscall
	or $t0, $0, $v0
	blt $t0, 20000000, while		# if($v0 < 200000) goto while
	ori $v0, $0, RESET_CORE_TIMER	# resetCoreTimer();
	syscall				# resetCoreTimer();
	ori $a0, $0, ' '
	ori $v0, $0, putChar
	syscall
	addi $t0, $t0, 1
	or $a0, $0, $t0
	ori $a1, $0, 10
	ori $v0, $0, printInt
	syscall
	j while
	jr $ra
