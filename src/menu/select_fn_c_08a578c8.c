/* Runs the handlers for state values 1..6 (by state - 1). */
extern void func_08A579A8(int self);
extern void func_08A57A18(int self);
extern void func_08A57B6C(int self);
extern void func_08A57BC4(int self);
extern void func_08A57C04(int self);
extern void func_08A57C5C(int self);
extern void func_08A57CB8(int self);
void func_08A578C8(int self) {
    switch (*(int *)(self + 0x2C) - 1) {
    case 0:
        func_08A579A8(self);
        func_08A57B6C(self);
        break;
    case 1:
        func_08A579A8(self);
        func_08A57A18(self);
        break;
    case 2:
        func_08A579A8(self);
        func_08A57BC4(self);
        break;
    case 3:
        func_08A579A8(self);
        func_08A57C04(self);
        break;
    case 4:
        func_08A57C5C(self);
        break;
    case 5:
        func_08A57CB8(self);
        func_08A579A8(self);
        break;
    }
}
