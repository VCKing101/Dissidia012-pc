/* Synthetic verifier fixture; contains no instructions copied from the game.
 * Both HI16 records precede their shared LO16 in the relocation table, while
 * the second high-load instruction occurs later in the instruction stream. */
.text
.globl relocation_order
.type relocation_order, @function
.set noreorder
relocation_order:
    b .Lsecond_high
    lui $t0, 0
.Llow:
    addiu $v0, $t0, 0
    jr $ra
    addiu $v1, $t1, 0
.Lsecond_high:
    lui $t1, 0
    b .Llow
    nop
.size relocation_order, . - relocation_order
.reloc relocation_order + 4, R_MIPS_HI16, relocation_target
.reloc relocation_order + 20, R_MIPS_HI16, relocation_target
.reloc relocation_order + 8, R_MIPS_LO16, relocation_target
.reloc relocation_order + 16, R_MIPS_LO16, relocation_target
