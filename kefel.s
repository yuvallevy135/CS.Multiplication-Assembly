.section .text
.globl  kefel
kefel:movl %edi, %eax
shl $4, %eax
movl %edi, %ecx
shl $1, %ecx
subl %ecx,%eax
ret
