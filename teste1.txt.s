.text
.globl main
main:
	move $fp, $sp

	# Alocando 2 Variaveis Locais na Pilha
	addiu $sp, $sp, -8
	# Lendo valor primitivo: 15
	li $s0, 15
	# Guardando variavel alterada: a
	sw $s0, -4($fp)
	# [Operador +]: Avaliando lado Esquerdo
	# Lendo variavel a
	lw $s0, -4($fp)
	# Empilhando esquerdo de (+)
	sw $s0, 0($sp)
	addiu $sp, $sp, -4
	# [Operador +]: Avaliando lado Direito
	# Lendo valor primitivo: 5
	li $s0, 5
	# Desempilhando pra efetuar calculo com Direito ($s0)
	lw $t1, 4($sp)
	addiu $sp, $sp, 4
	add $s0, $t1, $s0
	# [Fim Operador +]
	# Guardando variavel alterada: b
	sw $s0, -8($fp)
	.data
str_lit_0:
	.asciiz "O resultado eh: "
	.text
	# Syscall [4] - Escreva Cadeia Literaria
	li $v0, 4
	la $a0, str_lit_0
	syscall
	# Lendo variavel b
	lw $s0, -8($fp)
	# Syscall [1] - Imprime Inteiros
	li $v0, 1
	move $a0, $s0
	syscall
	.data
nl_lit_1:
	.asciiz "\n"
	.text
	li $v0, 4
	la $a0, nl_lit_1
	syscall

# Sair do Programa (Syscall Exit 10)
	li $v0, 10
	syscall
