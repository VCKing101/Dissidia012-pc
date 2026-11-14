void func_08A756A4(char *p, char *other);
void func_08A74334(char *p);
void func_08A72E5C(char *p);
void func_08A72760(char *p);
void func_08A72800(char *p);

void func_08A7BC10(char *self, char *other) {
    func_08A756A4(self + 4, other);
    func_08A74334(self + 0x430);
    func_08A72E5C(self + 0x440);
    func_08A72760(other + 0x184);
    func_08A72800(other + 0x184);
}
