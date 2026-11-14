/* Loads an effect file: opens it, checks the header and the version, then
   reads the body. Returns 0, or -1 to -6 for the step that failed. */
struct File088106AC {
    char data[0x10];
};

struct Effect088106AC {
    unsigned char loaded;
};

extern "C" {

extern int D_089D4780;
extern float D_089C6D98;

void func_08810964(File088106AC *file);
void func_0881099C(File088106AC *file, int flags);
int func_08810A20(File088106AC *file, int name, int mode);
void *func_08810A88(File088106AC *file, int size);
int func_089359E8(int heap);
int func_08935B50(void *header, int heap, int size);
int func_0881081C(Effect088106AC *self, File088106AC *file);

int func_088106AC(Effect088106AC *self, int name, int mode)
{
    File088106AC file;
    func_08810964(&file);
    if (func_08810A20(&file, name, mode) != 0) {
        func_0881099C(&file, 2);
        return -1;
    }
    void *header = func_08810A88(&file, 4);
    if (header == 0) {
        func_0881099C(&file, 2);
        return -2;
    }
    int heap = D_089D4780;
    if (func_08935B50(header, heap, func_089359E8(heap)) != 0) {
        func_0881099C(&file, 2);
        return -3;
    }
    float *version = (float *)func_08810A88(&file, 4);
    if (version == 0) {
        func_0881099C(&file, 2);
        return -4;
    }
    if (*version != D_089C6D98) {
        func_0881099C(&file, 2);
        return -5;
    }
    if (func_0881081C(self, &file) != 0) {
        func_0881099C(&file, 2);
        return -6;
    }
    self->loaded = 1;
    func_0881099C(&file, 2);
    return 0;
}

}
