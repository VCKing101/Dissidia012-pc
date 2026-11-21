/* Allocates a copy of a block into the object and runs the constructor over its payload. */
typedef struct Blk38 { char pad[0xC]; int *copy; int *payload; int *owner; } Blk38;
extern int *func_08842100(int size);
extern void func_08935524(void *dst, void *src, int size);
extern void func_08993560(void *obj, int a, int b, void *cb);
extern char func_08AC5570[];
void func_08AC6124(Blk38 *self, void *src, int size)
{
    int *blk;
    int *owner;
    self->copy = func_08842100(size);
    func_08935524(self->copy, src, size);
    blk = self->copy;
    self->payload = blk + 2;
    owner = 0;
    if (blk != 0) {
        func_08993560((char *)blk + 8, 1, 0x50, func_08AC5570);
        owner = blk;
    }
    self->owner = owner;
}
