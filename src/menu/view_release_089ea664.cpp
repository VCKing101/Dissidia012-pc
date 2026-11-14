/* Arcade course view: releases its three model handles and the loader
   group 0x15, then the view's own parts. */
typedef struct Loader29 {
    char pad_00[0x8];
    void *heap;
} Loader29;

typedef struct CourseView29 {
    char pad_00[0x8];
    void *handles[3];
} CourseView29;

extern "C" {
extern Loader29 *D_089D7830;
void func_088DAB44(void *handle);
void func_088D0370(Loader29 *loader, int group, int arg);
void func_0887B3B8(CourseView29 *view);
}

/* Inlined release of a loader group; the narrow parameters make SNC set the
   constant arguments before it loads D_089D7830. */
struct LoaderOps29 {
    static void Release(char group, char arg) {
        func_088D0370(D_089D7830, group, arg);
    }
};

extern "C" {

void func_089EA664(CourseView29 *view)
{
    for (int i = 0; i < 3; i++) {
        if (view->handles[i] != 0) {
            func_088DAB44(view->handles[i]);
            view->handles[i] = 0;
        }
    }
    LoaderOps29::Release(0x15, 1);
    D_089D7830->heap = 0;
    func_0887B3B8(view);
}
}
