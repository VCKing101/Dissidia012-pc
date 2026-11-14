struct A6Handles2 {
    char pad_00[0x40];
    int field_40[22];
};

extern "C" {
void func_088D9CFC(int handle);

void func_08A61618(A6Handles2 *self) {
    for (int i = 11; i < 22; i++) {
        func_088D9CFC(self->field_40[i]);
    }
}
}
