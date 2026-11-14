struct Owner0888F984 {
    unsigned char unknown_00[0x10];
    int items[5];
    int count;
};

extern "C" {
void func_0888EA84(int item);
void func_0888FC34(Owner0888F984 *self);

void func_0888F984(Owner0888F984 *self) {
    for (int i = 0; i < self->count; i++) {
        func_0888EA84(self->items[i]);
    }
    func_0888FC34(self);
}
}
