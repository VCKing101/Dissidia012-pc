/* Formats "talkevent/%s.evex" into the object's buffer at 0x3C from the name at 0x1710, clears that name, then calls func_088E8DF8. */
typedef struct Obj088e91e8 { unsigned char pad[0x3C]; char path[0x16D4]; char name[1]; } Obj088e91e8;
extern char D_089CC820[];
extern void func_0893476C(char *dst, char *fmt, char *name);
extern void func_088E8DF8(Obj088e91e8 *o);
void func_088E91E8(Obj088e91e8 *self)
{
    func_0893476C(self->path, D_089CC820, self->name);
    self->name[0] = 0;
    func_088E8DF8(self);
}
