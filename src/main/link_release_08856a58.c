typedef struct Link { int field_00; void *obj; char pad_08[0x18]; } Link;
typedef struct LinkOwner { char pad_00[0x74]; Link links[6]; int count; } LinkOwner;
extern void func_0887BA5C(void *obj);
void func_08856A58(LinkOwner *self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->links[i].obj != 0) {
            func_0887BA5C(self->links[i].obj);
            self->links[i].obj = 0;
        }
    }
    self->count = 0;
}
