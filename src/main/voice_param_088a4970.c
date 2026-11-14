/* Passes a parameter block to the voice's device handle. */
typedef struct Mixer088A4970 {
    int field_00;
    int field_04;
    void *device;
} Mixer088A4970;

typedef struct Voice088A4970 {
    char pad_00[0x10];
    int handle;
} Voice088A4970;

extern Mixer088A4970 *D_089D6D80;

void func_08941C8C(void *device, int handle, void *param);

void func_088A4970(Voice088A4970 *v, void *param)
{
    int handle = v->handle;
    func_08941C8C(D_089D6D80->device, handle, param);
}
