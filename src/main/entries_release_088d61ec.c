typedef struct Entry { void *obj; unsigned short flags; short pad; } Entry;
extern void func_088D617C(void *obj);

void func_088D61EC(Entry *e, int count)
{
    int i;
    for (i = 0; i < count; i++) {
        if ((e[i].flags & 1) == 0) {
            func_088D617C(e[i].obj);
        }
    }
}
