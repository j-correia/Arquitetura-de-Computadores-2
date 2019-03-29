	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.equ PUT_CHAR, 3		# $a0 = character
	.equ PRINT_INT, 6		# $a0 = val, $a1 = base
	.data
	.text
	.globl main
main:	li $t0, 0
while:	li $v0, READ_CORE_TIMER		# while(1){
	syscall
	bge $v0, 20000000, ewhile		# if($v0 => 20000000) goto ewhile
	j while
ewhile:	li $v0, RESET_CORE_TIMER
	syscall				# resetCoreTimer();
	li $a0, ' '
	li $v0, PUT_CHAR		# putChar(' ');
	syscall
	addi $t0, $t0, 1		# ++counter;
	move $a0, $t0
	li $a1, 10
	li $v0, PRINT_INT
	syscall				# printInt(++counter, 10);
	j while				# }
	jr $ra	
