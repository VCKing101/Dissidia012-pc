typedef struct MainMenuResourcePrefix {
    unsigned char unknown_0000[0xEE0];
    void *field_0EE0;
} MainMenuResourcePrefix;

extern void *D_089D727C;
extern void *func_0887AFBC(void *, int, int);
extern void func_088B1230(void *, void *, void *, int, int, int);

void func_088AD524(void *object, void *payload) {
    MainMenuResourcePrefix *self = object;
    void *entry = func_0887AFBC(self->field_0EE0, 17, 0);
    func_088B1230(D_089D727C, entry, payload, 0, 0xA3, 0);
}
