/* Preserve the overlay's fixed address. ELF packaging is reconstructed separately. */
INCLUDE build/ULUS10566/extra.undefined_funcs.txt
INCLUDE build/ULUS10566/extra.undefined_syms.txt
INCLUDE build/ULUS10566/extra.c_symbols.ld
SECTIONS
{
    .text 0x089DD4C0 : SUBALIGN(4)
    {
        INCLUDE build/ULUS10566/extra.sections.ld
    }
    /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.comment) *(.gnu.attributes) }
}
