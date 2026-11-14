/* Initialises the 5 icons and the 4 label(s). */
struct InitIcons08A73D98 {
    char field_00[5][4];
    char field_labels[4][4];
};

extern "C" void func_088D2F30(void *icon);
extern "C" void func_088D3334(void *label);

extern "C" {
void func_08A73D98(InitIcons08A73D98 *self)
{
    for (int i = 0; i < 5; i++) {
        char *icon = self->field_00[i];
        func_088D2F30(icon);
    }
    for (int i = 0; i < 4; i++) {
        func_088D3334(self->field_labels[i]);
    }
}
}
