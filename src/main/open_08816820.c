typedef struct Agent4Opener {
    unsigned char opened;
    unsigned char unknown_01[3];
    unsigned char field_04[4];
} Agent4Opener;

void func_088167F0(Agent4Opener *self);
int func_0881B044(void *part);

int func_08816820(Agent4Opener *self) {
    func_088167F0(self);
    if (func_0881B044(self->field_04) != 0) {
        return -2;
    }
    self->opened = 1;
    return 0;
}
