/* Draws the entry's name with one of three layouts (flag bit 0, or field 0x674 == 1). */
struct NameEntry08AA9800 {
    char pad_0000[0xC];
    void *field_00C;
    char field_010[0x674 - 0x10];
    int field_674;
    char pad_0678[0x7CC0 - 0x678];
    unsigned int field_7CC0;
};

struct NamePage08AA9800 {
    char pad_0000[0x7CFC];
    NameEntry08AA9800 *field_7CFC;
};

extern "C" {
extern int D_08AB0418;
extern int D_08AB0508;
extern int D_08AB02B0;
void func_088D3F00(void *dst, void *src, void *table);
void func_08AADEE0(NamePage08AA9800 *self);
void func_08AADBB0(NamePage08AA9800 *self);
}

struct NameDraw08AA9800 {
    static void draw(char *text, void *target, void *table) { func_088D3F00(target, text, table); }
};

extern "C" {
void func_08AA9800(NamePage08AA9800 *self)
{
    NameEntry08AA9800 *entry = self->field_7CFC;
    if ((entry->field_7CC0 & 1) != 0) {
        NameDraw08AA9800::draw(entry->field_010, entry->field_00C, &D_08AB0418);
        func_08AADEE0(self);
    } else if (entry->field_674 == 1) {
        NameDraw08AA9800::draw(entry->field_010, entry->field_00C, &D_08AB0508);
    } else {
        NameDraw08AA9800::draw(entry->field_010, entry->field_00C, &D_08AB02B0);
    }
    func_08AADBB0(self);
}
}
