typedef struct MotionSource {
    float rate;
    char pad_04[0x10];
    float duration;
} MotionSource;

typedef struct MotionTiming {
    unsigned char done;
    float elapsed;
    float duration;
    MotionSource *source;
} MotionTiming;

/* Start timing; a negative duration means the source's own. */
void func_08ABD194(void *self, float duration, MotionTiming *timing, MotionSource *source)
{
    timing->done = 0;
    timing->elapsed = 0.0f;
    timing->duration = duration < 0.0f ? source->duration : duration;
    timing->source = source;
}
