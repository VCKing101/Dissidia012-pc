typedef struct SlotTable4 {
    unsigned char active;
    char pad_01[0x3];
    void *data;
    char pad_08[0x4];
    int first[10];
    int second[10];
    int third[10];
    int fourth[10];
} SlotTable4;

extern "C" {

extern void func_08AB843C(SlotTable4 *self);

int func_08AB8458(SlotTable4 *self, void *data)
{
    func_08AB843C(self);
    int none = -1;
    if (data == 0) {
        return none;
    }
    for (int i = 0; i < 10; i++) {
        self->first[i] = none;
        self->second[i] = none;
        self->third[i] = none;
        self->fourth[i] = none;
    }
    self->data = data;
    self->active = 1;
    return 0;
}

}
