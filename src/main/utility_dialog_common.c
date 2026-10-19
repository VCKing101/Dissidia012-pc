typedef struct MainUtilityDialogCommon {
    unsigned int size;
    int language;
    int accept_button;
    int graphics_priority;
    int access_priority;
    int font_priority;
    int sound_priority;
    unsigned char unknown_001C[0x14];
} MainUtilityDialogCommon;
extern void *func_08935828(void *, int, unsigned int);
extern int func_08867B90(void);
extern int func_08867BB0(void);
void func_088B168C(void *unused, void *output, unsigned int size) {
    MainUtilityDialogCommon *self = output;
    func_08935828(self,0,0x30);
    self->size = size;
    self->language = func_08867B90();
    self->accept_button = func_08867BB0();
    self->graphics_priority = 17;
    self->access_priority = 19;
    self->font_priority = 18;
    self->sound_priority = 16;
}
