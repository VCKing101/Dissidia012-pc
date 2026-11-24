/* Runs the handlers for state 1, 2, 4, 6 or 7. */
extern void func_08A60BEC(int self);
extern void func_08A60C40(int self);
extern void func_08A60E8C(int self);
extern void func_08A60EE4(int self);
extern void func_08A60F24(int self);
void func_08A60B38(int self) {
    switch (*(int *)(self + 0x28)) {
    case 1:
        func_08A60E8C(self);
        func_08A60BEC(self);
        break;
    case 2:
        func_08A60C40(self);
        func_08A60BEC(self);
        break;
    case 4:
        func_08A60BEC(self);
        func_08A60EE4(self);
        break;
    case 6:
        func_08A60BEC(self);
        break;
    case 7:
        func_08A60F24(self);
        func_08A60BEC(self);
        break;
    }
}
