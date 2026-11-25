/* Runs func_088D9CFC on the handles of each row of the layout at +0x250 (two ranges, two more when flag 0x100 at +0x294 is set, and the range at +0x34), then on handles 0x5F..0x61 and clears flag 4. */
typedef struct Layout08A948F4 {
    char pad0[0x24];
    int a24;
    int a28;
    char pad1[0x34 - 0x2C];
    int a34;
    char pad2[4];
    int count;
} Layout08A948F4;
typedef struct Obj08A948F4 { char pad[0x250]; Layout08A948F4 lay; char pad1[0x294 - 0x250 - sizeof(Layout08A948F4)]; int flags; } Obj08A948F4;
extern void func_088D9CFC(int h);
void func_08A948F4(void *unused, Obj08A948F4 *obj, int *handles)
{
    Layout08A948F4 *p = &obj->lay;
    int i;
    for (i = 0; i < p->count; i++) {
        func_088D9CFC(handles[p->a24 + i]);
        func_088D9CFC(handles[p->a28 + i]);
        if (obj->flags & 0x100) {
            func_088D9CFC(handles[p->a28 + p->count + i]);
            func_088D9CFC(handles[p->a28 + p->count * 2 + i]);
        }
        func_088D9CFC(handles[p->a34 + i]);
    }
    func_088D9CFC(handles[0x5F]);
    func_088D9CFC(handles[0x60]);
    func_088D9CFC(handles[0x61]);
    obj->flags &= ~4;
}
