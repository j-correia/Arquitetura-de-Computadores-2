# int main(void)
# {
#	char c;
#	while(1)
#	{
#		c = getChar();
#		if( c == '\n')
#			break;
#		putChar(c)
#	}
#	return 1;
# }

	.equ getChar, 2
	.equ putChar, 3
	.data
	.text
	.globl main
main:	
while:	ori $v0, $0, getChar
	syscall
	or $t0, $0, $v0			# $t0 = getChar;
if:	beq $t0, '\n', ewhile	
eif:	ori $v0, $0, putChar
	or $a0, $0, $t0
	syscall				# putChar($t0);
	j while
ewhile:	or $v0, $0, 1
	jr $ra
