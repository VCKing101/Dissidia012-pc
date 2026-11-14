typedef struct FadeJob {
    void *target;
    int pad_04;
    int state;
    char pad_0C[0xC];

    static bool running(FadeJob *job) { return job->state != 0; }
    static unsigned char fading(FadeJob *job)
    {
        int result = 0;
        if (job->state == 2 || job->state == 3) {
            result = 1;
        }
        return result;
    }
} FadeJob;

typedef struct FadePanel {
    FadeJob jobs[3];
} FadePanel;

extern "C" {

extern void func_08ADF514(FadeJob *job);

/* Restart all three fades. */
void func_08ADFD3C(FadePanel *self)
{
    for (int i = 0; i < 3; i++) {
        func_08ADF514(&self->jobs[i]);
    }
}

}
