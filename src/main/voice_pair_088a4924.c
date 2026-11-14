/* Passes two values to the voice's handle unless the second is -1. */
typedef struct Mixer088A4924 {
    int field_00;
    int field_04;
    void *device;
} Mixer088A4924;

typedef struct Voice088A4924 {
    char pad_00[0x10];
    int handle;
} Voice088A4924;

extern Mixer088A4924 *D_089D6D80;

void func_08941C3C(void *device, int handle, int a, int b);

void func_088A4924(Voice088A4924 *v, int a, int b)
{
    int handle = v->handle;
    if (b != -1) {
        func_08941C3C(D_089D6D80->device, handle, a, b);
    }
}
