/* Sends message 7 (the menu info's field 0x210, the menu's id and two
   shorts) unless func_08AB6DAC reports the menu busy. */
struct Info08AB5008 {
    char pad_000[0x210];
    int field_210;
};

struct Menu08AB5008 {
    char pad_00[0x24];
    int field_24;
};

struct Message08AB5008 {
    short field_0;
    short field_2;
    short field_4;
    short field_6;
};

extern int func_08AB6DAC(struct Menu08AB5008 *menu);
extern struct Info08AB5008 *func_08AB6D58(struct Menu08AB5008 *menu);
extern void func_08A6FC24(void *target, int kind, void *data, int size);

void func_08AB5008(struct Menu08AB5008 *self, int first, int second)
{
    struct Message08AB5008 message;
    struct Info08AB5008 *info;
    if (func_08AB6DAC(self) == 1) {
        return;
    }
    info = func_08AB6D58(self);
    message.field_0 = info->field_210;
    message.field_2 = self->field_24;
    message.field_4 = first;
    message.field_6 = second;
    func_08A6FC24(info, 7, &message, 8);
}
