/* Switches a voice option on (1) or off. */
typedef struct Mixer088A483C {
    int field_00;
    int field_04;
    void *device;
} Mixer088A483C;

typedef struct Voice088A483C {
    char pad_00[0x10];
    int handle;
} Voice088A483C;

extern Mixer088A483C *D_089D6D80;

void func_08941BBC(void *device, int handle);
void func_08941BE0(void *device, int handle);

void func_088A483C(Voice088A483C *v, unsigned char on)
{
    void *device = D_089D6D80->device;
    int handle = v->handle;
    if (on == 1) {
        func_08941BBC(device, handle);
    } else {
        func_08941BE0(device, handle);
    }
}
