typedef struct CameraEvent {
    char pad_00[0x4];
    float a;
    float b;
    float c;
    float d;
    float e;
} CameraEvent;

extern void func_08AA3E44(float a, float b, float c, float d, float e, void *self, int mode);

int func_08AA3308(void *self, CameraEvent *event)
{
    if (event == 0) {
        return -1;
    }
    func_08AA3E44(event->a, event->b, event->c, event->d, event->e, self, 1);
    return 0;
}
