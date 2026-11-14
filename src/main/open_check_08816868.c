typedef struct Port08816868 {
    unsigned char active;
    unsigned char unknown_01[3];
    unsigned char field_04[4];
} Port08816868;

void func_08816E84(Port08816868 *self);
int func_0881B210(void *p);

int func_08816868(Port08816868 *self) {
    if (self->active == 0) {
        return -1;
    }
    func_08816E84(self);
    if (func_0881B210(self->field_04) != 0) {
        return -2;
    }
    return 0;
}
