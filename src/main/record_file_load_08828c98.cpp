/* Loads a record file: copies it into a buffer, checks its tag and version
   and reads its record table. Returns -1..-5 for bad input, no memory, a
   bad tag, a bad version or a bad table. */
struct Records08828C98 {
    unsigned char ready;
    char pad_01[3];
    char *data;
    void *heap;
    void *table;
    int count;
};

struct Stream08828C98 {
    static char *take(char **cur, int n)
    {
        char *p = *cur;
        *cur = p + n;
        return p;
    }
    static float take_float(char **cur)
    {
        float v = *(float *)*cur;
        *cur += 4;
        return v;
    }
};

extern "C" {

extern char *D_089D493C;
extern float D_089C78E8;

void func_08828C50(Records08828C98 *self);
char *func_0885C6CC(void *heap, int size, int align, int zero, int one, int flags);
void func_08935524(void *dst, const void *src, int size);
int func_08935B50(const char *a, const char *b, int n);
int func_08819AD0(void **table, int *count, char **cur, int size);

int func_08828C98(Records08828C98 *self, void *heap, const void *src, int size)
{
    func_08828C50(self);
    if (heap == 0 || src == 0 || size <= 0) {
        return -1;
    }
    int one = 1;
    self->data = func_0885C6CC(heap, size, 0, 0, one, 0);
    self->heap = heap;
    if (self->data == 0) {
        return -2;
    }
    func_08935524(self->data, src, size);
    char *cur = self->data;
    char *tag = Stream08828C98::take(&cur, 4);
    if (func_08935B50(tag, D_089D493C, 4) != 0) {
        return -3;
    }
    float version = Stream08828C98::take_float(&cur);
    if (D_089C78E8 != version) {
        return -4;
    }
    if (func_08819AD0(&self->table, &self->count, &cur, 8) != 0) {
        return -5;
    }
    self->ready = 1;
    return 0;
}

}
