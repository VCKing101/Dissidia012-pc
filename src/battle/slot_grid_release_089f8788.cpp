typedef struct SlotGrid {
    unsigned char active;
    char pad_01[0x3];
    void *field_04;
    void *heads[4];
    void *cells[4][4];
    void *tails[4];
} SlotGrid;

extern "C" {

extern void func_088DA060(void *handle);
extern void func_089F8710(SlotGrid *self);

void func_089F8788(SlotGrid *self)
{
    for (int i = 0; i < 4; i++) {
        if (self->tails[i] != 0) {
            func_088DA060(self->tails[i]);
            self->tails[i] = 0;
        }
        for (int j = 0; j < 4; j++) {
            if (self->cells[i][j] != 0) {
                func_088DA060(self->cells[i][j]);
                self->cells[i][j] = 0;
            }
        }
        if (self->heads[i] != 0) {
            func_088DA060(self->heads[i]);
            self->heads[i] = 0;
        }
    }
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_089F8710(self);
}

}
