/* Reloads a part marker's model for the owner's side and re-creates its
 * parts. */
typedef struct PartMarker {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    unsigned char parts[0x20];
    int owner;
    unsigned char pad_2c[4];
    void *source;
    int kind;
    unsigned char flag;
} PartMarker;

extern void func_089F7E40(void *parts);
extern void func_088DA060(void *model);
extern unsigned int func_08A22CA8(int owner);
extern void *func_088DAA40(void *source, int name, int flags);
extern void func_08A08584(PartMarker *marker);
extern int D_08A08890[2];

int func_08A087A8(PartMarker *marker, int kind, unsigned char flag)
{
    unsigned int side;

    if (!marker->open) {
        return -1;
    }
    func_089F7E40(marker->parts);
    if (marker->model != 0) {
        func_088DA060(marker->model);
        marker->model = 0;
    }
    side = func_08A22CA8(marker->owner);
    if (side >= 2) {
        return -2;
    }
    marker->model = func_088DAA40(marker->source, D_08A08890[side], 0);
    if (marker->model == 0) {
        return -3;
    }
    marker->kind = kind;
    marker->flag = flag;
    func_08A08584(marker);
    return 0;
}
