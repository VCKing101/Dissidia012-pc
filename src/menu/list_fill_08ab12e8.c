/* Resets the id list to a single 0, then appends 1..n for the n entries the
   lookup returns. */
struct ListOwner08AB12E8 {
    char pad_000[5];
    signed char field_005;
    char pad_006[5];
    signed char field_00B;
    char pad_00C[0x151 - 0xC];
    signed char field_151[0x43];
    char field_194[0x1586 - 0x194];
    signed char field_1586;
};

extern int D_09B6DF08;
extern void func_08AB6D58(void *source);
extern int func_08929EE8(char *out, int kind, int a, int b, void *table);

void func_08AB12E8(struct ListOwner08AB12E8 *self, int a, int b, void *source)
{
    signed char count;
    int n;
    int i;
    self->field_00B = 0;
    count = self->field_00B;
    self->field_151[count] = 0;
    self->field_00B = count + 1;
    func_08AB6D58(source);
    n = func_08929EE8(self->field_194, 0xF, a, b, &D_09B6DF08);
    i = 0;
    if (i < n) {
        count = self->field_00B;
        do {
            i++;
            self->field_151[count] = i;
            count++;
        } while (i < n);
        self->field_00B = count;
    }
    self->field_005 = self->field_1586;
}
