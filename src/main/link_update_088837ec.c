extern unsigned char D_089D69DC;
extern unsigned char D_089D69DD;
extern int D_089D69D4;
extern unsigned char D_09B49128[];
extern unsigned char D_09B49130[];

void func_08883D90(void *p);
void func_08883DF0(void *p);

void func_088837EC(void) {
    if (D_089D69DC == 0) {
        return;
    }
    func_08883D90(D_09B49128);
    func_08883DF0(D_09B49130);
    if (D_089D69D4 != 0) {
        return;
    }
    D_089D69DD = 0;
}
