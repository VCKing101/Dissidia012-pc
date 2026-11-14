typedef struct MenuGlobal {
    char pad_000[0x138];
    int field_138;
} MenuGlobal;

extern MenuGlobal *D_08A23FF0;
extern void func_0885C6CC(int handle, void *self, int a, int b, int c, int d);

void func_08A24090(void *self) {
    int none = 0;
    func_0885C6CC(D_08A23FF0->field_138, self, 0, 0, none, 0);
}
