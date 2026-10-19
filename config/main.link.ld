/* The huge VMA gap/BSS is intentionally absent from file-backed output. */
INCLUDE build/ULUS10566/main.symbols.ld
SECTIONS
{
    .segment0 0x08804040 : SUBALIGN(4)
    {
        INCLUDE build/ULUS10566/main.segment0.sections.ld
    }
    ASSERT(SIZEOF(.segment0) == 1938304, "Main segment 0 size mismatch")
    .segment1 0x08B63170 : SUBALIGN(4)
    {
        INCLUDE build/ULUS10566/main.segment1.sections.ld
    }
    ASSERT(SIZEOF(.segment1) == 256, "Main segment 1 size mismatch")
    /* Only audited, unreferenced weak inline definitions are allowed here. */
    /DISCARD/ : { *(.gnu.linkonce.t.*) *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.comment) *(.gnu.attributes) }
}
