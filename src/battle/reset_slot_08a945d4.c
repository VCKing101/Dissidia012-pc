/* Resets the entry to its defaults; with mode 1 also clears its three trailing ids to -1. */
typedef struct Slot38 { unsigned char active; char pad[3]; int a; int b; float c; float d; int e; int f[3]; } Slot38;
extern float D_08A94DC8;
int func_08A945D4(Slot38 *s, unsigned char mode)
{
    int i;
    int r = -1;
    if (s->active == 0) {
        return r;
    }
    float d;
    s->a = 0;
    s->b = r;
    s->c = -1.0f;
    d = D_08A94DC8;
    s->e = r;
    d = -d;
    s->d = d;
    if (mode == 1) {
        i = 0;
        do {
            s->f[i] = r;
            i = i + 1;
        } while (i < 3);
    }
    return 0;
}
