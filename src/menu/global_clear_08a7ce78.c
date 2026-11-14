typedef struct A6Game2 {
    char pad_00[0x1FC];
    int field_1FC;
} A6Game2;

extern A6Game2 **D_08A6D7E0;
void func_08A76710(char *p, char *other);
void func_08A772E0(char *p);
void func_08A72760(char *p);
void func_08A72800(char *p);

void func_08A7CE78(char *self, char *other) {
    func_08A76710(self + 4, other);
    func_08A772E0(self + 0x1E4);
    func_08A72760(other + 0x184);
    func_08A72800(other + 0x184);
    (*D_08A6D7E0)->field_1FC = 0;
}
