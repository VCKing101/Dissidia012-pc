/* Switches a voice option on (1) or off. */
typedef struct Mixer088A47E4 {
    int field_00;
    int field_04;
    void *device;
} Mixer088A47E4;

typedef struct Voice088A47E4 {
    char pad_00[0x10];
    int handle;
} Voice088A47E4;

extern Mixer088A47E4 *D_089D6D80;

void func_08941B2C(void *device, int handle);
void func_08941B50(void *device, int handle);

void func_088A47E4(Voice088A47E4 *v, unsigned char on)
{
    void *device = D_089D6D80->device;
    int handle = v->handle;
    if (on == 1) {
        func_08941B2C(device, handle);
    } else {
        func_08941B50(device, handle);
    }
}
