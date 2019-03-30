# void main(void)
# {
#	int value;
#	while(1)
#	{
#		printStr("\nIntroduza um numero (sinal e módulo: ");
#		value = readInt10();
#		printStr("\nValor lido em base 2: ");
#		printInt(value, 2);
#		printStr("\nValor lido em base 16: ");
#		printInt(value, 16);
#		printStr("\nValor lido em base 10 (unsigned): ");
#		printInt(value, 10);
#		printStr("\nValor lido em base 10 (signed): ");
#		printInt10(value);	
#	}
# }
	.equ printInt, 6 	# $a0 = val, $a1 = base
	.equ printStr, 8 	# $a0 = str
	.equ readInt10, 5
	.equ printInt10, 7
	.data
msg0:	.asciiz "\nIntroduza um número (sinal e módulo): "
msg1:	.asciiz "\nValor lido em base 2: "
msg2:	.asciiz "\nValor lido em base 16: "
msg3:	.asciiz "\nValor lido em base 10 (unsigned): "
msg4:	.asciiz "\nValor lido em base 10 (signed): "
	.text
	.globl main
main:	
while:	ori $v0, $0, printStr
	la $a0, msg0
	syscall			# printStr(msg0);
	ori $v0, $0, readInt10
	syscall
	or $t0, $0, $v0		# $t0 = readInt10();
	ori $v0, $0, printStr
	la $a0, msg1
	syscall			# printStr(msg1);
	ori $v0, $0, printInt
	or $a0, $0, $t0
	ori $a1, $0, 2		# printInt(value, 2);
	syscall
	ori $v0, $0, printStr
	la $a0, msg2
	syscall			# printStr(msg2);
	ori $v0, $0, printInt
	or $a0, $0, $t0
	ori $a1, $0, 16		# printInt(value, 16);
	syscall		
	ori $v0, $0, printStr
	la $a0, msg3
	syscall			# printStr(msg3);
	ori $v0, $0, printInt
	or $a0, $0, $t0
	ori $a1, $0, 10		# printInt(value, 10);
	syscall	
	ori $v0, $0, printStr
	la $a0, msg4
	syscall			# printStr(msg4);
	ori $v0, $0, printInt10
	or $a0, $0, $t0
	syscall			# printInt10(value)
	j while
	jr $ra
