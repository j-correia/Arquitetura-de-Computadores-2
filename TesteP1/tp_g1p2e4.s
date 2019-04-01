# #define STR_MAX_SIZE 20
# char *strcat(char *, char *);
# char *strcpy(char *, char *);
# int strlen(char *);
# int main(void)
# {
# 	static char str1[STR_MAX_SIZE + 1];
#	static char str2[STR_MAX_SIZE + 2];
#	static char str3[2*STR_MAX_SIZE + 1];
#	printStr("Introduza 2 strings: "
#	readStr(str1, STR_MAX_SIZE);
# 	readStr(str2, STR_MAX_SIZE);
#	printStr("Resultados:\n");
#	printInt(strlen(str1), 10);
# 	printInt(strlen(str2), 10);
#	strcpy(str3, str1);
#	printStr(strcat(str3, str2));
#	printInt10(strcmp(str1, str2));
#	return 0;
# }
	.equ STR_MAX_SIZE, 20
	.equ printInt, 6	# $a0 = value, $a1 = base
	.equ printInt10, 7
	.equ printStr, 8
	.equ readStr, 9		# $a0 = buffer, $a1 = nc
	.data
msg1:	.asciiz "Introduza 2 strings: "
msg2:	.asciiz "\nResultados:\n"
str1:	.space 21
str2:	.space 21
str3:	.space 41
	.text
	.globl main
main:	subu $sp, $sp, 20		# Salvaguarda de Contexto
	sw $ra, 0($sp)			# Salvaguarda de Contexto
	sw $s0, 4($sp)			# Salvaguarda de Contexto
	sw $s1, 8($sp)			# Salvaguarda de Contexto
	sw $s2, 12($sp)			# Salvaguarda de Contexto
	sw $s3, 16($sp)			# Salvaguarda de Contexto
	ori $v0, $0, printStr 		# printStr(msg1);
	la $a0, msg1			# printStr(msg1);
	syscall				# printStr(msg1);
	la $a0, str1			# str1 = readStr();
	ori $a1, $0, STR_MAX_SIZE	# str1 = readStr();
	ori $v0, $0, readStr		# str1 = readStr();
	syscall				# str1 = readStr();
	la $a0, str2			# str2 = readStr();
	ori $a1, $0, STR_MAX_SIZE	# str2 = readStr();
	ori $v0, $0, readStr		# str2 = readStr();
	syscall				# str2 = readStr();
	la $a0, msg2			# printStr(msg2);
	ori $v0, $0, printStr		# printStr(msg2);
	syscall				# printStr(msg2);
	la $a0, str1			# strlen(str1);
	jal strlen			# strlen(str1);
	or $a0, $0, $v0			# printInt($v0, 10);
	ori $a1, $0, 10			# printInt($v0, 10);
	ori $v0, $0, printInt		# printInt($v0, 10);
	syscall				# printInt($v0, 10);
	la $a0, str2			# strlen(str2);
	jal strlen			# strlen(str2);
	or $a0, $0, $v0			# printInt($v0, 10);
	ori $a1, $0, 10			# printInt($v0, 10);
	ori $v0, $0, printInt		# printInt($v0, 10);
	syscall				# printInt($v0, 10);
	la $a0, str3			# strcpy(str3, str1);
	la $a1, str1			# strcpy(str3, str1);
	jal strcpy			# strcpy(str3, str1);
	la $a0, str3			# strcat(str3, str2);
	la $a1, str2			# strcat(str3, str2);
	jal strcat			# strcat(str3, str2);
	ori $v0, $0, printStr		# printStr(str3);
	la $a0, str3			# printStr(str3);
	syscall				# printStr(str3);
	la $a0, str1			# strcmp(str1, str2);
	la $a1, str2			# strcmp(str1, str2);
	jal strcmp			# strcmp(str1, str2);
	or $a0, $0, $v0			# printInt10(strcmp(str1, str2);
	ori $v0, $0, printInt10		# printInt10(strcmp(str1, str2);
	syscall				# printInt10(strcmp(str1, str2);
	or $v0, $0, $0			# return 0;
	lw $ra, 0($sp)			# Reposição de Contexto
	lw $s0, 4($sp)			# Reposição de Contexto
	lw $s1, 8($sp)			# Reposição de Contexto
	lw $s2, 12($sp)			# Reposição de Contexto
	lw $s3, 16($sp)			# Reposição de Contexto
	addu $sp, $sp, 20		# Reposição de Contexto
	jr $ra
#
# String length
# int strlen(char *s)
# {
#	int len;
#	for(len = 0; *s != 0; len++, s++);
#	return len;
# }
#
# int strlen(char *s)
# sub-rotina terminal -- Sem Salvaguarda
# entrada: ($a0)	
# saida:   ($v0)
strlen:	ori $t0, $0, 0		# int len = 0;
for1:	lb $t1, 0($a0)	
	beq $t1, 0, efor1	# if( $a0 == 0) goto efor1
	addi $t0, $t0, 1	# len++;
	addi $a0, $a0, 1	# s++;
	j for1
efor1:	or $v0, $0, $t0		# return len;
 	jr $ra			#
#
# String concatenate
# char *strcat(char *dst, char *src)
# {
#	char *rp = dst;
#	for(; *dst != 0; dst++);
#	strcpy(dst, src);
#	return rp;
# }
# int strcat(char *dst, char *src)
# sub-rotina intermédia -- Com Salvaguarda
# entrada: ($a0, $a1)
# saída:   ($v0)
strcat:	subu $sp, $sp, 20	# reservar espaço na stack
	sw $ra, 0($sp)		# salvaguarda de contexto
	sw $s0, 4($sp)		# salvaguarda de contexto
	sw $s1, 8($sp)		# salvaguarda de contexto
	sw $s2, 12($sp)		# salvaguarda de contexto
	sw $s3, 16($sp)		# salvaguarda de contexto
	or $s0, $0, $a0		# char *rp = dst;
for2:	lb $t0, 0($a0)	
	beq $t0, 0, efor2	# if( $t0 == 0) goto efor2
	addi $a0, $a0, 1	# dst++;
	j for2			# 
efor2:	jal strcpy		# strcpy($a0, $a1)
	or $v0, $0, $s0		# return rp;
	lw $ra, 0($sp)		# reposição de contexto
	lw $s0, 4($sp)		# reposição de contexto
	lw $s1, 8($sp)		# reposição de contexto
	lw $s2, 12($sp)		# reposição de contexto
	lw $s3, 16($sp)		# reposição de contexto
	addu $sp, $sp, 20	# limpar stack
	jr $ra			# }
# 
# String copy
# char *strcpy(char *dst, char *src)
# {
#	char *rp = dst;
#	for(; (*dst = *src) != 0; dst++, src++);
#	return rp;
# }	
# char *strcpy(char *dst, char *src)
# sub-rotina terminal -- Sem Salvaguarda
# entrada:   ($a0, $a1)
# saída:     ($v0)
strcpy:	or $t0, $0, $a0		# char *rp = dst;
for3:	lb $t1, 0($a1)		# $t1 = src
	sb $t1, 0($a0)		# dst = $t1
	beq $t1, 0, efor3	# if(dst == 0) goto efor3
	addu $a0, $a0, 1	# dst++;
	addu $a1, $a1, 1	# src++;
	j for3
efor3: 	or $v0, $0, $t0		# return rp;
	jr $ra			# }
#
# String compare (alphabetically).
# Returned value is:
#  < 0 string "s1" is less than string "s2"
#  = 0 string "s1" is equal to string "s2"
#  > 0 string "s1" is greater than string "s2"
# int strcmp(char *s1, char *s2)
# {
# 	for(; (*s1 == *s2) && (*s1 != 0); s1++, s2++);
#	return(*s1 - *s2);
# }	
# int strcmp(char *s1, char *s2)
# sub-rotina terminal -- Sem Salvaguarda
# entrada:   ($a0, $a1)
# saída:     ($v0)
strcmp:	
for4:	lb $t0, 0($a0)		# $t0 = *$a0
	lb $t1, 0($a1)		# $t1 = *$a1
	bne $t0, $t1, efor4	# if( $t0 != $t1) goto efor4
	beq $t0, 0, efor4	# if( $t0 == 0) goto efor4
	addu $a0, $a0, 1	# *s1++;
	addu $a1, $a1, 1	# *s2++;
	j for4			# }
efor4:	lb $t0, 0($a0)		# $t0 = *s1 
	lb $t1, 0($a1)		# $t1 = *s2
	sub $v0, $t0, $t1	# return ($t0 - $t1)
	jr $ra			# }
