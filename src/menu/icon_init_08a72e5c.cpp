/* Initialises the 6 icons and the 1 label(s). */
struct InitIcons08A72E5C {
    char field_00[6][4];
    char field_labels[1][4];
};

extern "C" void func_088D2F30(void *icon);
extern "C" void func_088D3334(void *label);

extern "C" {
void func_08A72E5C(InitIcons08A72E5C *self)
{
    for (int i = 0; i < 6; i++) {
        char *icon = self->field_00[i];
        func_088D2F30(icon);
    }
    for (int i = 0; i < 1; i++) {
        func_088D3334(self->field_labels[i]);
    }
}
}
