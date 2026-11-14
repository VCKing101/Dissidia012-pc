/* Releases all 35 resource slots. */
struct Res088A3C48 {
    int field_00;
    int field_04;
    int field_08;
};

struct Bank088A3C48 {
    Res088A3C48 slots[35];
};

extern "C" {

void func_088A3A10(Res088A3C48 *r);
void func_088A3998(Res088A3C48 *r);

void func_088A3C48(Bank088A3C48 *b)
{
    for (int i = 0; i < 35; i++) {
        Res088A3C48 *r = &b->slots[i];
        func_088A3A10(r);
        func_088A3998(r);
    }
}

}
