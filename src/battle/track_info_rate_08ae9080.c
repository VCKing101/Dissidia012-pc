typedef struct TrackInfo {
    char pad_00[0x28];
    float rate;
} TrackInfo;

typedef struct TrackSource {
    char pad_00[0x44];
    float rate;
} TrackSource;

extern void func_08AE7EF8(TrackSource *src, TrackInfo *out);

void func_08AE9080(TrackSource *src, TrackInfo *out)
{
    func_08AE7EF8(src, out);
    out->rate = src->rate;
}
