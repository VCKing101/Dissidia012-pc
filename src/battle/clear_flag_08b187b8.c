// If func_08B18804(self) is non-zero, clears bit 5 of byte 0x14 of the record at +4 and returns 0; else returns -1.
extern int func_08B18804(void *);
typedef struct { char pad[0x14]; unsigned char b; } Rec08B187B8;
typedef struct { int pad; Rec08B187B8 *rec; } Obj08B187B8;
int func_08B187B8(Obj08B187B8 *self)
{
    if (func_08B18804(self) == 0) {
        return -1;
    }
    self->rec->b &= ~0x20;
    return 0;
}
