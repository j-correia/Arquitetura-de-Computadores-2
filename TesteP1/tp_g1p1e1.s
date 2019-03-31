# int main (void)
# {
# 	printStr("AC2 - DETPIC32\n");
#	return 0;
# }
	.equ PRINT_STR, 8
msg:	.asciiz "AC2 - DETPIC32\n"
	.text
	.globl main
main:	la $a0, msg			# $a0 = address[msg]
	ori $v0, $0, PRINT_STR		# $a1 = 8
	syscall				# printStr(msg)
	ori $v0, $0, 0			# return 0
	jr $ra				#
