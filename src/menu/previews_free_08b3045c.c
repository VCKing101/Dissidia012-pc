/* Releases the three previews and redraws the frame. */
struct State08B3045C {
    char pad_000[0x10];
    char field_010[0x490 - 0x10];
    char field_490[0x5F8 - 0x490];
    void *field_5F8;
    void *field_5FC;
    void *field_600;
};

struct Owner08B3045C {
    char pad_000[0x9D8];
    struct State08B3045C *field_9D8;
};

extern unsigned short D_08B31826[];
extern void func_0887BA5C(void *preview);
extern void func_088D428C(char *layout, char *cursor, unsigned short *table, int show);

void func_08B3045C(struct Owner08B3045C *self)
{
    if (self->field_9D8->field_5F8 != 0) {
        func_0887BA5C(self->field_9D8->field_5F8);
        self->field_9D8->field_5F8 = 0;
    }
    if (self->field_9D8->field_5FC != 0) {
        func_0887BA5C(self->field_9D8->field_5FC);
        self->field_9D8->field_5FC = 0;
    }
    if (self->field_9D8->field_600 != 0) {
        func_0887BA5C(self->field_9D8->field_600);
        self->field_9D8->field_600 = 0;
    }
    func_088D428C(self->field_9D8->field_010, self->field_9D8->field_490, D_08B31826, 1);
}
