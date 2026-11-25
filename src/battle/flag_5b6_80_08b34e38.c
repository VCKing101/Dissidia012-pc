// Tests bit 0x80 of the byte at offset 0x5B6 (sub-struct at 0x5B0, byte 6).
struct S08B34E38 { char pad[6]; unsigned char f; };
struct F08B34E38 { char pad[0x5B0]; struct S08B34E38 s; };
int func_08B34E38(struct F08B34E38 *self)
{
    struct S08B34E38 *p = &self->s;
    return (p->f & 0x80) != 0;
}
