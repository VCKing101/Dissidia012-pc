/* Returns the 0x38-byte table entry for the object's slot index. */
struct SlotObj { int pad; int index; };
extern char *D_089D479C;
char *func_08B3E8B0(struct SlotObj *self)
{
    return D_089D479C + self->index * 0x38;
}
