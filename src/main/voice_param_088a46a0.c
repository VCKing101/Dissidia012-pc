/* Passes a parameter block to the voice's device handle. */
typedef struct Mixer088A46A0 {
    int field_00;
    int field_04;
    void *device;
} Mixer088A46A0;

typedef struct Voice088A46A0 {
    char pad_00[0x10];
    int handle;
} Voice088A46A0;

extern Mixer088A46A0 *D_089D6D80;

void func_08941A14(void *device, int handle, void *param);

void func_088A46A0(Voice088A46A0 *v, void *param)
{
    int handle = v->handle;
    func_08941A14(D_089D6D80->device, handle, param);
}
