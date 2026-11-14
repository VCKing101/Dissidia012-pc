typedef struct BankSlot { char data[0x60]; } BankSlot;
typedef struct BankFilter { char data[0x50]; } BankFilter;
typedef struct BankItem { char data[0x40]; } BankItem;
typedef struct FilterBank {
    BankSlot slots[2];
    BankFilter filters[2];
    BankItem items[2];
    BankSlot extra;
    int field_240;
    char pad_244[0x10];
    int field_254;
    float field_258;
    float field_25C;
    float field_260;
    int field_264;
} FilterBank;
extern float D_089C9720;
extern float D_089C9724;
extern float D_089C9728;
extern void func_08993560(void *array, int count, int size, void (*ctor)(void *));
extern void func_0887205C(void *p);
extern void func_088724CC(void *p);
extern void func_08872ADC(void *p);
FilterBank *func_08872B94(FilterBank *self)
{
    func_08993560(self->slots, 2, 0x60, func_0887205C);
    func_08993560(self->filters, 2, 0x50, func_088724CC);
    func_08993560(self->items, 2, 0x40, func_08872ADC);
    func_0887205C(&self->extra);
    self->field_240 = -1;
    self->field_254 = 0;
    self->field_258 = D_089C9720;
    self->field_25C = D_089C9724;
    self->field_260 = D_089C9728;
    self->field_264 = 0;
    return self;
}
