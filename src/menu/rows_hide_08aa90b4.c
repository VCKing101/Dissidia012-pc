typedef struct HideRows {
    char pad_00[0x10];
    int field_10[0x20];
} HideRows;

typedef struct HideOwner {
    char pad_0000[0x7CFC];
    HideRows *field_7CFC;
} HideOwner;

extern void func_088D9CFC(int handle);

void func_08AA90B4(HideOwner *self, unsigned short first)
{
    int last = 6;
    int i;
    for (i = 1; i <= last; i++) {
        func_088D9CFC(self->field_7CFC->field_10[first + i]);
    }
}
