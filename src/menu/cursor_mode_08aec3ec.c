/* Mode 4 copies the current row's value and enters cursor mode 2; otherwise
   frees the preview, redraws the cursor and leaves cursor mode. */
struct Row08AEC3EC {
    short field_0;
    char pad_2[10];
};

struct State08AEC3EC {
    char pad_0000[0x400];
    char field_400[0x880 - 0x400];
    char field_880[0x8A0 - 0x880];
    void *field_8A0;
    char pad_08A4[0x1924 - 0x8A4];
    unsigned int field_1924;
    char pad_1928[0x1960 - 0x1928];
    int field_1960;
    char pad_1964[0x19E0 - 0x1964];
    struct Row08AEC3EC field_19E0[(0x1D12 - 0x19E0) / 12];
    char pad_1D10[2];
    unsigned char field_1D12[0x23E4 - 0x1D12];
    int field_23E4;
    short field_23E8;
};

struct Screen08AEC3EC {
    char pad_0000[0x23F0];
    struct State08AEC3EC *field_23F0;
};

extern unsigned short D_08AEFD26[];
extern void func_0887BA5C(void *preview);
extern void func_088D436C(char *layout, char *cursor, unsigned short *table);

void func_08AEC3EC(struct Screen08AEC3EC *self)
{
    if ((self->field_23F0->field_1924 & 4) != 0) {
        struct Row08AEC3EC *row = &self->field_23F0->field_19E0[self->field_23F0->field_1D12[self->field_23F0->field_1960]];
        self->field_23F0->field_23E8 = row->field_0;
        self->field_23F0->field_23E4 = 2;
    } else {
        if (self->field_23F0->field_8A0 != 0) {
            func_0887BA5C(self->field_23F0->field_8A0);
            self->field_23F0->field_8A0 = 0;
        }
        func_088D436C(self->field_23F0->field_400, self->field_23F0->field_880, D_08AEFD26);
        self->field_23F0->field_23E4 = 0;
    }
}
