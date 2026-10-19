/* Battle shares its VMA with the other mutually exclusive overlays. */
INCLUDE build/ULUS10566/battle.symbols.ld
SECTIONS
{
    .text 0x089DD4C0 : SUBALIGN(4)
    {
        INCLUDE build/ULUS10566/battle.sections.ld
    }
    ASSERT(SIZEOF(.text) == 1596080, "Battle payload size mismatch")
    /* Only audited, unreferenced weak inline definitions are allowed here. */
    /DISCARD/ : { *(.gnu.linkonce.t.*) *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.comment) *(.gnu.attributes) }
}
