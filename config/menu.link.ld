/* Menu shares its VMA with the other mutually exclusive overlays. */
INCLUDE build/ULUS10566/menu.symbols.ld
SECTIONS
{
    .text 0x089DD4C0 : SUBALIGN(4)
    {
        INCLUDE build/ULUS10566/menu.sections.ld
    }
    ASSERT(SIZEOF(.text) == 1555864, "Menu payload size mismatch")
    /* Only audited, unreferenced weak inline definitions are allowed here. */
    /DISCARD/ : { *(.gnu.linkonce.t.*) *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.comment) *(.gnu.attributes) }
}
