/* Passes a parameter block to the voice's device handle. */
typedef struct Mixer088A457C {
    int field_00;
    int field_04;
    void *device;
} Mixer088A457C;

typedef struct Voice088A457C {
    char pad_00[0x10];
    int handle;
} Voice088A457C;

extern Mixer088A457C *D_089D6D80;

void func_089419E8(void *device, int handle, void *param);

void func_088A457C(Voice088A457C *v, void *param)
{
    int handle = v->handle;
    func_089419E8(D_089D6D80->device, handle, param);
}
