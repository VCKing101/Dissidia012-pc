/* Clears a state word, runs the common step and then two steps chosen by the mode (0..4), then the finish step. */
extern void func_08ABC828(int self, int a);
extern void func_08ABC868(int self);
void func_08ABC608(int self) {
    *(int *)(*(int *)(self + 0x14C) + 0xB08) = 0;
    func_08ABC828(self, 5);
    switch (*(unsigned *)(*(int *)(self + 0x14C) + 0xA68)) {
    case 0:
        *(unsigned char *)(*(int *)(self + 0x14C) + 0xB4E) = 1;
        func_08ABC828(self, 8);
        func_08ABC828(self, 7);
        break;
    case 1:
        func_08ABC828(self, 10);
        func_08ABC828(self, 9);
        break;
    case 2:
        func_08ABC828(self, 12);
        func_08ABC828(self, 11);
        break;
    case 3:
        func_08ABC828(self, 14);
        func_08ABC828(self, 13);
        break;
    case 4:
        func_08ABC828(self, 16);
        func_08ABC828(self, 15);
        break;
    }
    func_08ABC868(self);
}
