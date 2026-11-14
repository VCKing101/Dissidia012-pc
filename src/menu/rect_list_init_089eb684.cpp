/* Rectangle list: five cleared slots and a copy of the caller's rectangles. */
struct RectSlot {
    int field_00;
    int field_04;
    int field_08;
};

struct ShortRect {
    short x;
    short y;
    short w;
    short h;
};

struct RectList {
    void *field_00;
    int field_04;
    RectSlot field_08[5];
    ShortRect field_44[5];
    int field_6C;
    int field_70;
};

extern "C" int D_089EC280;

extern "C" {
RectList *func_089EB684(RectList *self, ShortRect *rects, int count)
{
    self->field_00 = &D_089EC280;
    self->field_04 = 0;
    self->field_6C = count;
    self->field_70 = 0;
    for (int i = 0; i < 5; i++) {
        RectSlot *slot = &self->field_08[i];
        slot->field_00 = 0;
        slot->field_04 = 0;
        slot->field_08 = 0;
    }
    for (int i = 0; i < self->field_6C; i++) {
        self->field_44[i] = rects[i];
    }
    return self;
}
}
