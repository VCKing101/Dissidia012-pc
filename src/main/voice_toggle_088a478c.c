/* Switches a voice option on (1) or off. */
typedef struct Mixer088A478C {
    int field_00;
    int field_04;
    void *device;
} Mixer088A478C;

typedef struct Voice088A478C {
    char pad_00[0x10];
    int handle;
} Voice088A478C;

extern Mixer088A478C *D_089D6D80;

void func_08941AF4(void *device, int handle);
void func_08941B10(void *device, int handle);

void func_088A478C(Voice088A478C *v, unsigned char on)
{
    void *device = D_089D6D80->device;
    int handle = v->handle;
    if (on == 1) {
        func_08941AF4(device, handle);
    } else {
        func_08941B10(device, handle);
    }
}
