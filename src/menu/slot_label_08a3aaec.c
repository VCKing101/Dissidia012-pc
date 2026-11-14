typedef struct LabelSlot {
    char data[0x90];
} LabelSlot;

typedef struct LabelTable {
    int field_00;
    LabelSlot field_04[16];
} LabelTable;

extern char D_08A3BE18[];
extern int func_0893476C(char *buffer, const char *format, int value);
extern int func_08869AB4(LabelSlot *slot, char *text);

int func_08A3AAEC(LabelTable *self, int id)
{
    int slot;
    char text[0x20];
    if (id < 0 || id >= 0x200) {
        return 0;
    }
    slot = (id - 1) / 32;
    if (slot < 0 || slot >= self->field_00) {
        return 0;
    }
    func_0893476C(text, D_08A3BE18, id);
    return func_08869AB4(&self->field_04[slot], text);
}
