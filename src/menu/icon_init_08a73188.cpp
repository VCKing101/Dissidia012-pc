/* Initialises the 3 icons and the 2 label(s). */
struct InitIcons08A73188 {
    char field_00[3][4];
    char field_labels[2][4];
};

extern "C" void func_088D2F30(void *icon);
extern "C" void func_088D3334(void *label);

extern "C" {
void func_08A73188(InitIcons08A73188 *self)
{
    for (int i = 0; i < 3; i++) {
        char *icon = self->field_00[i];
        func_088D2F30(icon);
    }
    for (int i = 0; i < 2; i++) {
        func_088D3334(self->field_labels[i]);
    }
}
}
