/* Loads the side's mark model, dropping the one it held. */
struct SideMark {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
};

extern void func_088DA060(void *model);
extern unsigned int func_08A22CA8(unsigned int side);
extern void *func_088DAA40(void *source, int name, int flags);
extern int D_08A157C8[2];

int func_08A156E4(struct SideMark *mark, unsigned int side, void *source)
{
    unsigned int kind;

    if (!mark->open) {
        return -1;
    }
    if (side >= 2 || source == 0) {
        return -2;
    }
    if (mark->model != 0) {
        func_088DA060(mark->model);
        mark->model = 0;
    }
    kind = func_08A22CA8(side);
    if (kind >= 2) {
        return -3;
    }
    mark->model = func_088DAA40(source, D_08A157C8[kind], 0);
    if (mark->model == 0) {
        return -4;
    }
    return 0;
}
