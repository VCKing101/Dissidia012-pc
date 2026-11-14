/* Calls the device function for the voice's handle. */
typedef struct Mixer088A4A6C {
    int field_00;
    int field_04;
    void *device;
} Mixer088A4A6C;

typedef struct Voice088A4A6C {
    char pad_00[0x10];
    int handle;
} Voice088A4A6C;

extern Mixer088A4A6C *D_089D6D80;

void func_08941268(void *device, int handle);

void func_088A4A6C(Voice088A4A6C *v)
{
    int handle = v->handle;
    func_08941268(D_089D6D80->device, handle);
}
