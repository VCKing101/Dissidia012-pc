/* Places the base element, then shows each of the two arrows at its anchor or hides it (anchor < 0). */
struct ArrowSet {
    char field_00[0x4];
    char field_04[2][0x4];
    int field_0C[2];
};

extern "C" {
void func_088D2FB8(ArrowSet *part, void *base, int anchor, int mode);
void func_088D3450(char *part, ArrowSet *base, int anchor);
void func_088D34B8(char *part);
void func_088D34E0(char *part);

void func_08A74BF8(ArrowSet *self, void *base, int anchor)
{
    func_088D2FB8(self, base, anchor, 3);
    for (int i = 0; i < 2; i++) {
        if (self->field_0C[i] >= 0) {
            func_088D3450(self->field_04[i], self, self->field_0C[i]);
            func_088D34B8(self->field_04[i]);
        } else {
            func_088D34E0(self->field_04[i]);
        }
    }
}
}
