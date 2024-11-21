.globl _start

.text
_start:
        li x29, 32
        li x4, 4
        vsetvl x5, x29, x4
        li x8, 0

        li x28, 0
        li x6, 1
        li x7, 32
loop:
        sw x6, 0x423(x8)
        addi x6, x6, 1
        addi x28, x28, 1
        add x8, x8, x4
        blt x28, x7, loop

        li x6, 3
        li x27, 0
        li x9, 0
loop2:
        sw x6, 0x623(x9)
        addi x6, x6, 3
        addi x27, x27, 1
        add x9, x9, x4
        blt x27, x7, loop2

        vlw.v v1, 0x423(x0)
        vlw.v v2, 0x623(x0)

        vmul.vv v4, v3, v2
        vadd.vv v3, v1, v2

        vsw.v v3, 0x423(x0)
        vsw.v v4, 0x623(x0)

        nop
        nop
        ebreak

.data