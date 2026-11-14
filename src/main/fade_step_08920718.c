/* Marks a finished fade as done, otherwise advances it. */
typedef struct FadeState {
    int done;                   /* 0x00 */
    char pad_04[0x9];
    unsigned char phase;        /* 0x0D */
} FadeState;

extern void func_08920788(FadeState *fade);

void func_08920718(FadeState *fade)
{
    if (fade->phase == 2) {
        fade->done = 1;
    } else {
        func_08920788(fade);
    }
}
