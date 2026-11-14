/* Arcade course list: releases its two model handles and the loader
   group 0x15. */
typedef struct Loader29 {
    char pad_00[0x8];
    int field_08;
} Loader29;

typedef struct CourseList29 {
    void *handles[2];
} CourseList29;

extern "C" {
extern Loader29 *D_089D7830;
void func_088DAB44(void *handle);
void func_088D0370(Loader29 *loader, int group, int arg);
}

/* Inlined release of a loader group; the narrow parameters make SNC set the
   constant arguments before it loads D_089D7830. */
struct LoaderOps29 {
    static void Release(char group, char arg) {
        func_088D0370(D_089D7830, group, arg);
    }
};

extern "C" void func_089E9FE4(CourseList29 *list)
{
    {
        int i;
        for (i = 0; i < 2; i++) {
        if (list->handles[i] != 0) {
            func_088DAB44(list->handles[i]);
            list->handles[i] = 0;
        }
    }
    }
    LoaderOps29::Release(0x15, 1);
    D_089D7830->field_08 = 0;
}
