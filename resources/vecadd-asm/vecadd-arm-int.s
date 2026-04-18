; Adds two integer vectors.
; n.b.: currently buggy on getting the array label addresses loaded right
; this runs on https://bkhmsi.github.io/ARMThumb_Sim/#/
; as of 2021/6/10 except for problems as noted.
; data segment is stored starting at 0x200, so you have to display values
; higher than that to see the array.

.code
bl _start
swi 6 

_start:
   mov r0, #4 ; our counter
   ldr r1, =array1
   ldr r2, =array2
   ldr r3, =array3
loop:
   ldr r4, [r1], #0
   ldr r5, [r2], #0
   add r6, r4, r5
   str r6, [r3], #0
   add r1, r1, #4
   add r2, r2, #4
   add r3, r3, #4
   sub r0, #1
   cmp r0, #0
   bne loop
   bx  lr
.data 
    dummy:  .word 0xffffffff
    array1: .word 1 
    a1dum:  .word 2
            .word 3
            .word 4
    array2: .word 5
    a2dum:  .word 6
            .word 7
            .word 8
    array3: .word 0
    a3dum:  .word 0
            .word 0
            .word 0
            .word 0xdeadbeef
            .word 0xffffffff
hello: .asciiz "Hello World"
