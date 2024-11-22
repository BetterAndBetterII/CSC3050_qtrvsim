.globl _start

.text
_start:
        li x11, 32
        li x4, 4
        vsetvl x5, x11, x4
	    li x6, 0
        li x10, 0
init:
		add x6, x6, x4
		addi x10, x10, 1
        blt x10, x5, init

        li x8, 0

        li x12, 0
        li x9, 1
        li x7, 42
loop:
        sw x9, 0x623(x8)
        addi x9, x9, 1
        addi x12, x12, 1
		add x8, x8, x4
        blt x12, x7, loop

        li x13, 3
        li x15, 0
	    li x14, 0
loop2:
        sw x13, 0x423(x14)
        addi x13, x13, 3
        addi x15, x15, 1
		add x14, x14, x4
        blt x15, x7, loop2

	    li x16, 0
loop3:
	    vsetvl x5, x7, x4
resetvi:

        vlw.v v1, 0x623(x16)
        vlw.v v2, 0x423(x16)
	    vmul.vv v3, v1, v2
	    vadd.vv v4, v3, v1

        vsw.v v3, 0x623(x16)
        vsw.v v4, 0x423(x16)

	    sub x7, x7, x5
	    add x16, x16, x6
	    blt x0, x7, loop3

        nop
        nop
        ebreak

.data