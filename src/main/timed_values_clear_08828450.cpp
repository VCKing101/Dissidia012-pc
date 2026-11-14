extern "C" {
typedef struct TimedValue { int id; float time; } TimedValue;
extern TimedValue D_09AFB850[4];
extern TimedValue D_09AFB870[2];
extern TimedValue D_09AFB880[2];
void func_08828450(void)
{
    for (int i = 0; i < 4; i++) {
        D_09AFB850[i].id = 0;
        D_09AFB850[i].time = 0.0f;
    }
    for (int i = 0; i < 2; i++) {
        D_09AFB870[i].id = 0;
        D_09AFB870[i].time = 0.0f;
    }
    for (int i = 0; i < 2; i++) {
        D_09AFB880[i].id = 0;
        D_09AFB880[i].time = 0.0f;
    }
}
}
