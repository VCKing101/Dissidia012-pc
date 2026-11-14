/* Arcade menu part: releases its nine model handles and the loader group
   0x15, then the base part. */
typedef struct Loader29 {
    char pad_00[0x8];
    void *heap;
} Loader29;

typedef struct ArcadePart29 {
    char pad_00[0x8];
    void *handles[9];
} ArcadePart29;

extern "C" {
extern Loader29 *D_089D7830;
void func_088DAB44(void *handle);
void func_088D0370(Loader29 *loader, int group, int arg);
void func_0887B3B8(ArcadePart29 *self);
}

/* Inlined release of a loader group; the narrow parameters make SNC set the
   constant arguments before it loads D_089D7830. */
struct LoaderOps29 {
    static void Release(char group, char arg) {
        func_088D0370(D_089D7830, group, arg);
    }
};

extern "C" {

void func_089EC894(ArcadePart29 *self)
{
    for (int i = 0; i < 9; i++) {
        if (self->handles[i] != 0) {
            func_088DAB44(self->handles[i]);
            self->handles[i] = 0;
        }
    }
    LoaderOps29::Release(0x15, 1);
    func_0887B3B8(self);
}
}
