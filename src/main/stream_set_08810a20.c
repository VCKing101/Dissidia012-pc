typedef struct Agent4Source {
    unsigned char active;
    unsigned char unknown_01[3];
    void *data;
    int size;
} Agent4Source;

void func_08810A04(Agent4Source *self);

int func_08810A20(Agent4Source *self, void *data, int size) {
    func_08810A04(self);
    if (data == 0 || size < 0) {
        return -1;
    }
    self->data = data;
    self->size = size;
    self->active = 1;
    return 0;
}
