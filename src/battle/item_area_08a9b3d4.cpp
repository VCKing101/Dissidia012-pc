struct ItemBody;

struct ItemSlot {
    char pad_00[0x4];
    ItemBody *body;
    char pad_08[0x28];
    int state;
};

struct ItemCheck {
    unsigned char active;
    char pad_001[0x157];
    int gauge;
    char pad_15C[0x4];
    int id;
};

extern "C" {
extern int D_08ABBDF8;
extern void *func_08ABB4B4(int key);
extern int func_08AB78E8(void *area);
extern int func_08A6587C(int gauge, int level);
extern ItemSlot *func_08AB3264(int id);
extern void *func_08AB3294(int id);
extern int func_08A7ED68(void *unit);
extern int func_08835778(ItemBody *body);
extern int func_088BF4CC(int list, int id);
}

struct ItemRules {
    static unsigned char has(int value) { return value != 0; }
};

extern "C" {

/* 1 inside the area when the gauge is full (or the unit is idle) and the unit holds item 0x12E. */
bool func_08A9B3D4(ItemCheck *self)
{
    if (self->active == 0) {
        return false;
    }
    if (func_08AB78E8(func_08ABB4B4(D_08ABBDF8)) == 0) {
        return false;
    }
    if (func_08A6587C(self->gauge, 3) != 1) {
        if (func_08AB3264(self->id)->state != 2 || func_08A7ED68(func_08AB3294(self->id)) != 0) {
            return false;
        }
    }
    ItemBody *body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return false;
    }
    if (ItemRules::has(func_088BF4CC(func_08835778(body), 0x12E)) == 0) {
        return false;
    }
    return true;
}

}
