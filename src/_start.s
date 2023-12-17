# if you need to modify your entry point behavior, do it here
#	.section .entrypoint
#	.global	_start
#	.weak	_start
#	.type	_start,	@function
#_start:
#	xor	%rbp,	%rbp
#	call	main
#	.size	_start,	.-_start
#
#	.section .text.main
#	.weak	main
#	.type	main,	@function
#main:	mov	$231,	%eax
#	xor	%edi,	%edi
#	syscall
#	.size	main, .-main
