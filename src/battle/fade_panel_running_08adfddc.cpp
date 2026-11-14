typedef struct FadeJob {
    void *target;
    int pad_04;
    int state;
    char pad_0C[0xC];

    static bool running(int state) { return state != 0; }
    static unsigned char fading(int state)
    {
        int result = 0;
        if (state == 2 || state == 3) {
            result = 1;
        }
        return result;
    }
} FadeJob;

typedef struct FadePanel {
    FadeJob jobs[3];
} FadePanel;

extern "C" {

/* 1 while any fade is running. */
int func_08ADFDDC(FadePanel *self)
{
    int result = 0;
    for (int i = 0; i < 3; i++) {
        if (FadeJob::running(self->jobs[i].state) == 1) {
            result = 1;
            break;
        }
    }
    return result;
}

}
