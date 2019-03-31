# int main(void)
# {
# 	char c;
#	while(1)
#	{
#		while((c=inkey())==0);
#		if(c=='\n')
#			break;
#		printStr("Key pressed\n");
#	}
#	return 0;
# }
	.equ PRINT_STR, 8
	.equ INKEY, 1
	.data
msg:	.asciiz "Key pressed\n"
	.text
	.globl main
main:	ori $v0, $0, INKEY
	syscall
while:	beq $v0, 0, main 		# if($v0 == 0) goto main;
	beq $v0, '\n', endw		# if($v0 == '\n') goto endw
	la $a0, msg			# printStr(msg)
	ori $v0, $0, PRINT_STR		# printStr(msg)
	syscall				# printStr(msg)
	j main				# } goto main
endw:	ori $v0, $0, 0			# return 0;
	jr $ra
