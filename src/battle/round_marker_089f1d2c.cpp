/* Round marker: releases its model once the side's end motion has played,
 * and reloads the model for the owner's side. */
struct MarkerModel {
    unsigned char pad_00[0xC];
    short motion;
    unsigned char pad_0e[0x12];
    unsigned short flags;

    static bool hidden(MarkerModel *model) { return (model->flags & 0x20) != 0; }
};

struct RoundMarker {
    unsigned char open;
    unsigned char pad_01[3];
    MarkerModel *model;
    int owner;
    void *source;
};

extern "C" {

extern unsigned int func_08A22CA8(int owner);
extern void func_088DA060(MarkerModel *model);
extern MarkerModel *func_088DAA40(void *source, int name, int flags);
extern int D_089F1FF0[2];
extern int D_089F2000[2];

int func_089F1D2C(RoundMarker *marker)
{
    if (!marker->open) {
        return -1;
    }
    unsigned int side = func_08A22CA8(marker->owner);
    if (side >= 2) {
        return -2;
    }
    if (marker->model != 0 && marker->model->motion == D_089F2000[side] && MarkerModel::hidden(marker->model) == 1) {
        if (marker->model != 0) {
            func_088DA060(marker->model);
            marker->model = 0;
        }
    }
    return 0;
}

int func_089F1DE0(RoundMarker *marker)
{
    if (!marker->open) {
        return -1;
    }
    if (marker->model != 0) {
        func_088DA060(marker->model);
        marker->model = 0;
    }
    unsigned int side = func_08A22CA8(marker->owner);
    if (side >= 2) {
        return -2;
    }
    marker->model = func_088DAA40(marker->source, D_089F1FF0[side], 0);
    if (marker->model == 0) {
        return -3;
    }
    func_089F1D2C(marker);
    return 0;
}

}
