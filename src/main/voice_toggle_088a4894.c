/* Switches a voice option on (1) or off. */
typedef struct Mixer088A4894 {
    int field_00;
    int field_04;
    void *device;
} Mixer088A4894;

typedef struct Voice088A4894 {
    char pad_00[0x10];
    int handle;
} Voice088A4894;

extern Mixer088A4894 *D_089D6D80;

void func_08941B74(void *device, int handle);
void func_08941B98(void *device, int handle);

void func_088A4894(Voice088A4894 *v, unsigned char on)
{
    void *device = D_089D6D80->device;
    int handle = v->handle;
    if (on == 1) {
        func_08941B74(device, handle);
    } else {
        func_08941B98(device, handle);
    }
}
