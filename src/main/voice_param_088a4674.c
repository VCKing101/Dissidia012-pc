/* Passes a parameter block to the voice's device handle. */
typedef struct Mixer088A4674 {
    int field_00;
    int field_04;
    void *device;
} Mixer088A4674;

typedef struct Voice088A4674 {
    char pad_00[0x10];
    int handle;
} Voice088A4674;

extern Mixer088A4674 *D_089D6D80;

void func_08941A70(void *device, int handle, void *param);

void func_088A4674(Voice088A4674 *v, void *param)
{
    int handle = v->handle;
    func_08941A70(D_089D6D80->device, handle, param);
}
