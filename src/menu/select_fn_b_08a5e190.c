/* Runs the handlers for state values 1..6 (by state - 1). */
extern void func_08A5E300(int self);
extern void func_08A5E354(int self);
extern void func_08A5E3AC(int self);
extern void func_08A5E55C(int self);
extern void func_08A5E59C(int self);
extern void func_08A5E5F4(int self);
void func_08A5E190(int self) {
    switch (*(int *)(self + 0x28) - 1) {
    case 0:
        func_08A5E300(self);
        func_08A5E354(self);
        break;
    case 1:
        func_08A5E3AC(self);
        func_08A5E300(self);
        break;
    case 2:
        func_08A5E300(self);
        func_08A5E55C(self);
        break;
    case 3:
        func_08A5E300(self);
        func_08A5E59C(self);
        break;
    case 4:
        func_08A5E5F4(self);
        break;
    case 5:
        func_08A5E300(self);
        break;
    }
}
