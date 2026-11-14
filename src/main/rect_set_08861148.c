/* Sets rectangle idx of the layer when it is ready: -1 otherwise. */
typedef struct Rect08861148 {
    float x;
    float y;
    float w;
    float h;
    int field_10;
    int field_14;
} Rect08861148;

typedef struct Layer08861148 {
    char pad_000[0x40C];
    Rect08861148 rects[1];
} Layer08861148;

int func_08861050(Layer08861148 *layer);

int func_08861148(Layer08861148 *layer, int idx, float x, float y, float w, float h)
{
    if (func_08861050(layer) == 0) {
        return -1;
    }
    layer->rects[idx].x = x;
    layer->rects[idx].y = y;
    layer->rects[idx].w = w;
    layer->rects[idx].h = h;
    return 0;
}
