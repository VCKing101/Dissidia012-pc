/* Copies a 40-byte parameter block into the object at 0x15C; -1 when the source is null. */
typedef struct Par38 { float a; float b; int c[8]; } Par38;
typedef struct Obj38 { char pad[0x15C]; Par38 par; } Obj38;
int func_08A97878(Obj38 *self, Par38 *src)
{
    if (src == 0) {
        return -1;
    } else {
        self->par = *src;
        return 0;
    }
}
