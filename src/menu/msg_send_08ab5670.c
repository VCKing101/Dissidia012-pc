/* Sends message 15 (the 0x1A-byte record plus the menu's id) unless the menu is busy. */
typedef struct RecordMessage {
    char field_00[0x1A];
    short field_1A;
} RecordMessage;

typedef struct RecordMenu {
    char pad_00[0x24];
    int field_24;
} RecordMenu;

extern int func_08AB6DAC(RecordMenu *menu);
extern void *func_08AB6D58(RecordMenu *menu);
extern void func_08935524(void *dst, const void *src, int size);
extern void func_08A6FC24(void *target, int kind, void *data, int size);

void func_08AB5670(RecordMenu *self, void *record)
{
    RecordMessage message;
    void *target;
    if (func_08AB6DAC(self) == 1) {
        return;
    }
    target = func_08AB6D58(self);
    message.field_1A = self->field_24;
    func_08935524(message.field_00, record, 0x1A);
    func_08A6FC24(target, 0xF, &message, 0x1C);
}
