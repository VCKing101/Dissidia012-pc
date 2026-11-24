/* Constructs a list node via func_08B44B78 then sets its vtable and three fields (last one float zero). */
extern void *func_08B44B78(void *self, void *owner, int value);
extern char D_08B470C8[];
typedef struct { char pad[0x14]; char *vt; char pad2[0x18]; int a; int b; float c; } Obj;
Obj *func_08B46A70(Obj *self, void *owner, int value) {
    func_08B44B78(self, owner, value);
    self->vt = D_08B470C8;
    self->a = 0;
    self->b = 0;
    self->c = 0.0f;
    return self;
}
