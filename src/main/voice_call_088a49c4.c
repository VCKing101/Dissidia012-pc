/* Calls the device function for the voice's handle. */
typedef struct Mixer088A49C4 {
    int field_00;
    int field_04;
    void *device;
} Mixer088A49C4;

typedef struct Voice088A49C4 {
    char pad_00[0x10];
    int handle;
} Voice088A49C4;

extern Mixer088A49C4 *D_089D6D80;

void func_08941C20(void *device, int handle);

void func_088A49C4(Voice088A49C4 *v)
{
    int handle = v->handle;
    func_08941C20(D_089D6D80->device, handle);
}
