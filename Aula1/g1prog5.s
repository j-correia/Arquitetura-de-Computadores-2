# #define STR_MAX_SIZE 20
# char *strcat(char *, char *);
# char *strcpy(char *, char *);
# int strln(char *);
# int main(void)
# {
# 	static char str1[STR_MAX_SIZE + 1];
#	static char str2[STR_MAX SIZE + 1];
#	static char str3[2*STR_MAX_SIZE +1];
#	printStr("Introduza 2 strings: ");
#	readStr(str1, STR_MAX_SIZE);
#	readStr(str2, STR_MAX_SIZE);
#	printSTR("Resultados:\n");
#	printInt(strlen(str1), 10);
#	printInt(strlen(str2), 10);
#	strcpy(str3, str1);
#	printStr(strcat(str3, str2));
#	printInt10(strcmp(str1, str2));
#	return 0;
# }
#
	.equ readStr, 9		# $a0 = buffer, $a1 = nc
	.equ printStr, 8
	.equ printInt10, 7
	.equ printInt, 6	# $a0 = val, $a1 = base
	.equ STR_MAX_SIZE, 20
	.data
str1:	.asciiz "Introduza 2 strings: "
str2:	.asciiz "\nResultados:\n"
	#.align 2
srd1:	.space 21
srd2:	.space 21
srs1:	.space 41
	.text
	.globl main
main:	subu $sp, $sp, 20	# reservar espaço na stack
	sw $ra, 0($sp)		# salvaguarda de contexto
	sw $s0, 4($sp)		# salvaguarda de contexto
	sw $s1, 8($sp)		# salvaguarda de contexto
	sw $s2, 12($sp)		# salvaguarda de contexto
	sw $s3, 16($sp)		# salvaguarda de contexto
	la $a0, str1		# printStr(str1);
	ori $v0, $0, printStr	# printStr(str1);
	syscall			# printStr(str1);
	la $a0, srd1		# rsd1 = readStr();
	ori $a1, STR_MAX_SIZE	# rsd1 = readStr();
	ori $v0, $0, readStr	# rsd1 = readStr();
	syscall 		# rsd1 = readStr();
	la $a0, srd2		# rsd2 = readStr();
	ori $a1, STR_MAX_SIZE	# rsd2 = readStr();
	ori $v0, $0, readStr	# rsd2 = readStr();
	syscall			# rsd2 = readStr();
	la $a0, str2		# printStr(str2);
	ori $v0, $0, printStr	# printStr(str2);
	syscall			# printStr(str2);
	la $a0, srd1		# printInt( strlen(str1), 10);
	jal strlen		# printInt( strlen(str1), 10);
	or $a0, $0, $v0		# printInt( strlen(str1), 10);
	ori $a1, $0, 10		# printInt( strlen(str1), 10);
	ori $v0, $0, printInt	# printInt( strlen(str1), 10);
	syscall			# printInt( strlen(str1), 10);
	la $a0, srd2		# printInt( strlen(str2), 10);
	jal strlen		# printInt( strlen(str2), 10);
	or $a0, $0, $v0		# printInt( strlen(str2), 10);
	ori $a1, $0, 10		# printInt( strlen(str2), 10);
	ori $v0, $0, printInt	# printInt( strlen(str2), 10);
	syscall			# printInt( strlen(str2), 10);
	la $a0, srs1		# strcpy( str3, str1);
	la $a1, srd1		# strcpy( str3, str1);
	jal strcpy		# strcpy( str3, str1);
	la $a0, srs1		# strcat( str3, str2);
	la $a1, srd2		# strcat( str3, str2);
	jal strcat		# strcat( str3, str2);
	la $a0, srs1		# printStr( strcat( str3, str2))
	ori $v0, $0, printStr	# printStr( strcat( str3, str2))
	syscall			# printStr( strcat( str3, str2))
	la $a0, srd1		# strcmp( str1, str2)
	la $a1, srd2		# strcmp( str1, str2)
	jal strcmp		# strcmp( str1, str2)
	or $a0, $0, $v0		# printInt10( strcmp( str1, str2))
	ori $v0, $0, printInt10	# printInt10( strcmp( str1, str2))
	syscall			# printInt10( strcmp( str1, str2))
	or $v0, $0, $0		# return 0;
	lw $ra, 0($sp)		# reposição de contexto
	lw $s0, 4($sp)		# reposição de contexto
	lw $s1, 8($sp)		# reposição de contexto
	lw $s2, 12($sp)		# reposição de contexto
	lw $s3, 16($sp)		# reposição de contexto
	addu $ra, $ra, 20	# limpar stack
	jr $ra			# }
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
