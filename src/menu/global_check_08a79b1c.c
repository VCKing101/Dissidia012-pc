typedef struct A6Gate {
    char pad_00[4];
    unsigned char field_04;
} A6Gate;

extern int *D_08A6D7E0;
void func_08A6FB68(void);
void func_088F69F8(void);
void func_08A79E68(char *p, int v);

void func_08A79B1C(A6Gate *self, char *other) {
    if (self->field_04 == 1) {
        if (*D_08A6D7E0 != 0) {
            func_08A6FB68();
        }
        func_088F69F8();
        func_08A79E68(other + 0x1B0, 0);
    }
}
