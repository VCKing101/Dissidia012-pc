/* Initialises the seven icons and the three labels. */
typedef struct InitIcons {
    char field_00[7][4];
    char field_1C[3][4];
} InitIcons;

extern "C" void func_088D2F30(void *icon);
extern "C" void func_088D3334(void *label);

extern "C" {

void func_08A72528(InitIcons *self)
{
    for (int i = 0; i < 7; i++) {
        char *icon = self->field_00[i];
        func_088D2F30(icon);
    }
    for (int i = 0; i < 3; i++) {
        func_088D3334(self->field_1C[i]);
    }
}
}
