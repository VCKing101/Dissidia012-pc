extern "C" {
/* Loads the sound's data: 1 on success, 0 (and the error flag) on failure. */
typedef struct Sound088A3274 {
    int field_00;
    int file;
    int handle;
    unsigned int flags;
} Sound088A3274;

void func_088A33C0(void);
int func_088A3D30(int handle, int file, void (*done)(void), Sound088A3274 *s);

bool func_088A3274(Sound088A3274 *s)
{
    int ok = func_088A3D30(s->handle, s->file, func_088A33C0, s);
    unsigned int flags = s->flags;
    if (ok == 1) {
        s->flags = flags & ~1;
        return true;
    }
    s->flags = flags | 1;
    return false;
}
}
