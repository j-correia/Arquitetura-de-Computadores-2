# void main(void)
# {
#	int value;
#	while(1)
#	{
#		printStr("\nIntroduza um número (Sinal e Módulo): ");
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
	.equ readInt10, 5
	.equ printInt, 6	# $a0 = value, $a1 = base
	.equ printInt10, 7
	.equ printStr, 8
	.data
msg1:	.asciiz "\nIntroduza um número (Sinal e Módulo): "
msg2:	.asciiz "\nValor lido em base 2: "
msg3:	.asciiz "\nValor lido em base 16: "
msg4:	.asciiz "\nValor lido em base 10 (unsigned): "
msg5:	.asciiz "\nValor lido em base 10 (signed): "
	.text
	.globl main
main:	ori $v0, $0, printStr		# printStr(msg1);
	la $a0, msg1			# printStr(msg1);
	syscall				# printStr(msg1);
	ori $v0, $0, readInt10		# $t0 = readInt10;
	syscall				# $t0 = readInt10;
	or $t0, $0, $v0			# $t0 = readInt10;
	ori $v0, $0, printStr		# printStr(msg2);
	la $a0, msg2			# printStr(msg2);
	syscall				# printStr(msg2);
	or $a0, $0, $t0			# printInt(value, 2);
	ori $a1, $0, 2			# printInt(value, 2);
	ori $v0, $0, printInt		# printInt(value, 2);
	syscall				# printInt(value, 2);
	ori $v0, $0, printStr		# printStr(msg3);
	la $a0, msg3			# printStr(msg3);
	syscall				# printStr(msg3);
	or $a0, $0, $t0			# printInt(value, 16);
	ori $a1, $0, 16			# printInt(value, 16);
	ori $v0, $0, printInt		# printInt(value, 16);
	syscall				# printInt(value, 16);
	ori $v0, $0, printStr		# printStr(msg4);
	la $a0, msg4			# printStr(msg4);
	syscall				# printStr(msg4);
	or $a0, $0, $t0			# printInt(value, 10);
	ori $a1, $0, 10			# printInt(value, 10);
	ori $v0, $0, printInt		# printInt(value, 10);
	syscall				# printInt(value, 10);
	ori $v0, $0, printStr		# printStr(msg5);
	la $a0, msg5			# printStr(msg5);
	syscall				# printStr(msg5);
	or $a0, $0, $t0			# printInt10(value);
	ori $v0, $0, printInt10		# printInt10(value);
	syscall				# printInt10(value);
	j main 				# ?while(1)?
	jr $ra
