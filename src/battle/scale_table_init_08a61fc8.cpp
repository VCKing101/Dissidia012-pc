/* Ten float scales after an active flag; func_08A61FC8 resets them to 2. */
typedef struct ScaleTable {
    unsigned char active;
    char pad_01[0x3];
    float scales[10];
} ScaleTable;

extern "C" {

extern void func_08A61FAC(ScaleTable *self);

int func_08A61FC8(ScaleTable *self)
{
    func_08A61FAC(self);
    for (int i = 0; i < 10; i++) {
        self->scales[i] = 2.0f;
    }
    self->active = 1;
    return 0;
}

}
