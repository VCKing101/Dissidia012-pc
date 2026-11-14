/* Opens the sound's stream: 1 on success, 0 (and the error flag) on failure. */
typedef struct Sound088A310C {
    int field_00;
    int file;
    int handle;
    int size;
    unsigned int flags;
} Sound088A310C;

extern int D_089D6C88;

void func_088A3264(void);
int func_088A16B0(int heap, int file, int size, float start, float volume, void (*done)(void), Sound088A310C *s);

int func_088A310C(Sound088A310C *s)
{
    s->handle = func_088A16B0(D_089D6C88, s->file, s->size, 0.0f, 1.0f, func_088A3264, s);
    unsigned int flags = s->flags;
    if (s->handle != -1) {
        s->flags = flags & ~1;
        return 1;
    }
    s->flags = flags | 1;
    return 0;
}
