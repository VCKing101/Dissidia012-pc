/* Sets up the panel, copies the source into the second label and links both labels. */
struct LinkPanel {
    char pad_00[0x8];
    char field_08[4];
    char field_0C[2][4];
};

extern "C" void func_08A73400(LinkPanel *self);
extern "C" void func_088D321C(void *label, void *source);
extern "C" void func_088D32F8(void *label, void *other);
extern "C" void func_088D3374(void *label, int a, int b);
extern "C" void func_088D3068(void *group, void *label);

extern "C" {
void func_08A73370(LinkPanel *self, void *source)
{
    func_08A73400(self);
    func_088D321C(self->field_0C[1], source);
    func_088D32F8(self->field_0C[0], self->field_0C[1]);
    for (int i = 0; i < 2; i++) {
        func_088D3374(self->field_0C[i], 1, 3);
        func_088D3068(self->field_08, self->field_0C[i]);
    }
}
}
