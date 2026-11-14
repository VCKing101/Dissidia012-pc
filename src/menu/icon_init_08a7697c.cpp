/* Initialises the 2 icons and the 4 labels. */
struct InitIcons08A7697C {
    char field_00[2][4];
    char field_labels[4][4];
};

extern "C" void func_088D2F30(void *icon);
extern "C" void func_088D3334(void *label);

extern "C" {
void func_08A7697C(InitIcons08A7697C *self)
{
    for (int i = 0; i < 2; i++) {
        char *icon = self->field_00[i];
        func_088D2F30(icon);
    }
    for (int i = 0; i < 4; i++) {
        func_088D3334(self->field_labels[i]);
    }
}
}
