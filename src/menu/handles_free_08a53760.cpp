struct A6Handles {
    int pad_00[6];
    int field_18[5];
};

extern "C" {
void func_088D9CFC(int handle);

void func_08A53760(A6Handles *self) {
    for (int i = 3; i < 8; i++) {
        func_088D9CFC(self->field_18[i - 3]);
    }
}
}
