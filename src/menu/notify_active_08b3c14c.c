/* If the record at self+0xCC has its byte at 0x10 set, calls func_0889F2D4(D_089D6C30, record->first). */
struct Rec { int first; char pad[0xC]; unsigned char active; };
struct Obj { char pad[0xCC]; struct Rec *rec; };
extern void *D_089D6C30;
extern void func_0889F2D4(void *a, int b);
void func_08B3C14C(struct Obj *self)
{
    struct Rec *r = self->rec;
    if (r->active == 0) { return; }
    func_0889F2D4(D_089D6C30, r->first);
}
