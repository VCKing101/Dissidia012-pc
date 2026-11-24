/* Runs the handlers for state values 1..5 (by state - 1): most start with the common routine; value 3 (state 4) runs only the last one. */
extern void func_08A5D078(int self);
extern void func_08A5D0EC(int self);
extern void func_08A5D144(int self);
extern void func_08A5D184(int self);
extern void func_08A5D1DC(int self);
extern void func_08A5D228(int self);
void func_08A5CFC0(int self) {
    switch (*(int *)(self + 0x28) - 1) {
    case 0:
        func_08A5D078(self);
        func_08A5D0EC(self);
        break;
    case 1:
        func_08A5D078(self);
        func_08A5D228(self);
        break;
    case 2:
        func_08A5D078(self);
        func_08A5D144(self);
    case 3:
        func_08A5D078(self);
        func_08A5D184(self);
        break;
    case 4:
        func_08A5D1DC(self);
        break;
    }
}
