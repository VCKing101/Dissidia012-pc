typedef struct Slot { unsigned short field_00; unsigned short first; unsigned short second; } Slot;
typedef struct Layout { Slot slots[2]; } Layout;
typedef struct Model { char data_00[0xE]; short pose; } Model;
typedef struct Owner { void *resource; int field_04; int field_08; Model *model; int field_10; int field_14; int layout; unsigned int flags; } Owner;
extern Layout D_089C9F70[];
extern void func_088D9F5C(Model *model, int pose, int now);

void func_08884898(Owner *o, unsigned char clear)
{
    Model *model = o->model;
    if (model != 0) {
        Slot *slot = &D_089C9F70[o->layout].slots[0];
        o->flags |= 1;
        if (clear) {
            func_088D9F5C(model, -1, 1);
        } else {
            func_088D9F5C(model, slot->second, 1);
            o->model->pose = -1;
        }
    }
}
