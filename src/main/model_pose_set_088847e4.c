typedef struct Slot { unsigned short field_00; unsigned short first; unsigned short second; } Slot;
typedef struct Layout { Slot slots[2]; } Layout;
typedef struct Model { char data_00[0xE]; unsigned short pose; } Model;
typedef struct Owner { void *resource; int field_04; int field_08; Model *model; int field_10; int field_14; int layout; unsigned int flags; } Owner;
extern Layout D_089C9F70[];
extern Model *func_088DAA68(void *resource);
extern void func_088D9F5C(Model *model, int pose, int now);

void func_088847E4(Owner *o, unsigned char alternate)
{
    Model *model = o->model;
    if (model == 0 && o->resource != 0) {
        model = o->model = func_088DAA68(o->resource);
    }
    if (model != 0) {
        Slot *slot = &D_089C9F70[o->layout].slots[0];
        o->flags |= 1;
        if (alternate) {
            func_088D9F5C(model, slot->first, 1);
        } else {
            func_088D9F5C(model, slot->field_00, 1);
            {
                Model *m = o->model;
                m->pose = slot->first;
            }
        }
    }
}
