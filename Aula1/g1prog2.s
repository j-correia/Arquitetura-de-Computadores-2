# int main(void)
# {
# 	char c;
# 	while(1)
# 	{
# 		while((c = inkey()) == 0);
# 		if(c == '\n')
# 			break;
# 		printStr("Key Pressed\n");
# 	}
# 	return 0;
# }
	.equ print_str, 8
	.equ inkey, 1
	.data
msg:	.asciiz "Key pressed\n"
	.text
	.globl main
main:
while: 	
while2:	ori $v0, $0, inkey
	syscall			
	or $t0, $0, $v0		# $t0 = inkey();
	bne $t0, 0, if		#
	j while2
if:	bne $t0, '\n', eif
eif:	ori $v0, $0, print_str
	la $a0, msg
	syscall
	j while
	or $v0, $0, $0
	jr $ra
