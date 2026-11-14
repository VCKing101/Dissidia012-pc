struct A6Pairs {
    char pad_00[8];
    char field_08[4];
    char field_0C[2][4];
};

extern "C" {
void func_088D30E4(char *a, char *b);
void func_088D3334(char *a);

void func_08A73400(A6Pairs *self) {
    for (int i = 0; i < 2; i++) {
        func_088D30E4(self->field_08, self->field_0C[i]);
        func_088D3334(self->field_0C[i]);
    }
}
}
