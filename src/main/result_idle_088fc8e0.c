/* True when the result screen has no pending job at 0x1114. */
typedef struct ResultScreen {
    char pad_0000[0x1114];
    void *job;
} ResultScreen;

typedef struct ResultOwner {
    char pad_00[0x30];
    ResultScreen *screen;
} ResultOwner;

int func_088FC8E0(ResultOwner *owner)
{
    if (owner->screen->job == 0) {
        return 1;
    }
    return 0;
}
