# int main(void)
# {
# 	char c;
#	while(1)
#	{
#		if(c=='\n')
#			break;
#		putChar(c);
# 	}
#	return 1;
# }

	.equ inkey, 1
	.equ getChar, 2
	.equ putChar, 3
	.data
	.text
	.globl main
main:	ori $v0, $0, getChar	# $v0 = getChar();
	syscall			# $v0 = getChar();
	beq $v0, '\n', ewhile	# if($v0 == '\n') goto ewhile;
	or $a0, $0, $v0		# putChar($v0)
	ori $v0, $0, putChar	# putChar($v0)
	syscall			# putChar($v0)
	j main			# goto "while"
ewhile:	ori $v0, $0, 1
	jr $ra
