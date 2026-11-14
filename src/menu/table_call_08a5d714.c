typedef struct A6Row16 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
} A6Row16;

extern A6Row16 D_08A5DC38[];
int func_08A5D654(char *self);
void func_08A603C0(char *p, int v);

void func_08A5D714(char *self) {
    int i = func_08A5D654(self);
    A6Row16 *r = &D_08A5DC38[i];
    func_08A603C0(self + 0x50, r->field_04);
}
