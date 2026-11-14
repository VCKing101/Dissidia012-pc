typedef struct SideGridA {
    char data[0x38];
} SideGridA;

typedef struct SideGridB {
    char data[0x34];
} SideGridB;

typedef struct SideGrid {
    char pad_00[0xC];
    void *field_0C;
    void *field_10;
    void *cells[2][4];
    SideGridA first[2];
    SideGridB second[2];
} SideGrid;

extern "C" {

extern void func_088DA060(void *handle);
extern void func_08A08224(SideGridB *entry);
extern void func_089F82C8(SideGridA *entry);
extern void func_08A0ECB0(SideGrid *self);

void func_08A0ED3C(SideGrid *self)
{
    for (int i = 0; i < 2; i++) {
        func_08A08224(&self->second[i]);
        func_089F82C8(&self->first[i]);
        for (int j = 0; j < 4; j++) {
            if (self->cells[i][j] != 0) {
                func_088DA060(self->cells[i][j]);
                self->cells[i][j] = 0;
            }
        }
    }
    if (self->field_10 != 0) {
        func_088DA060(self->field_10);
        self->field_10 = 0;
    }
    if (self->field_0C != 0) {
        func_088DA060(self->field_0C);
        self->field_0C = 0;
    }
    func_08A0ECB0(self);
}

}
