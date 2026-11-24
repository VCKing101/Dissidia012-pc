// Loads sound/snd_menu.scd into obj+0x1C once (from the pack if given, else by name); returns 1 when it loaded.
typedef struct { char pad[0x1C]; int handle; } Menu44;
extern int D_089D6C80;
extern char D_089CBE2C[], D_089CBE40[];
extern int func_088A0D90(int, const char *);
extern int func_088A0DBC(int, const char *, int, int, int);
extern int *func_08869AB4(int, const char *);
int func_088D1604(Menu44 *m, int pack)
{
    int r = 0;
    if (m->handle == -1) {
        char *path = D_089CBE2C;
        char *name = D_089CBE40;
        if (pack == 0) {
            r = func_088A0D90(D_089D6C80, path);
            m->handle = r;
        } else {
            int *e = func_08869AB4(pack, name);
            if (e != 0) {
                r = func_088A0DBC(D_089D6C80, path, e[1], e[2], 1);
                m->handle = r;
            }
        }
        r = 1;
    }
    return r;
}
