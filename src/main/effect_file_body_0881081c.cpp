/* Reads the body of an effect file: a header word, the version, and the
   two counted arrays (28-byte entries, then bytes). Returns 0 or the
   failing step (-1 to -5). */
struct File0881081C;

extern "C" void *func_08810A88(File0881081C *file, int size);

struct EffectFile0881081C {
    char pad_00[8];
    int version;
    int entry_count;
    void *entries;
    char pad_14[4];
    int data_size;
    void *data;

    static int read_array(File0881081C *file, int *count, void **items, int size)
    {
        int *p = (int *)func_08810A88(file, 4);
        if (p == 0) {
            return -2;
        }
        int n = *p;
        *count = n;
        if (n < 0) {
            return -3;
        }
        void *buffer = func_08810A88(file, n * size);
        *items = buffer;
        if (buffer == 0) {
            return -4;
        }
        return 0;
    }
};

extern "C" {

int func_0881081C(EffectFile0881081C *self, File0881081C *file)
{
    if (file == 0) {
        return -1;
    }
    if (func_08810A88(file, 4) == 0) {
        return -2;
    }
    int *version = (int *)func_08810A88(file, 4);
    if (version == 0) {
        return -3;
    }
    self->version = *version;
    if (EffectFile0881081C::read_array(file, &self->entry_count, &self->entries, 28) != 0) {
        return -4;
    }
    if (EffectFile0881081C::read_array(file, &self->data_size, &self->data, 1) != 0) {
        return -5;
    }
    return 0;
}

}
