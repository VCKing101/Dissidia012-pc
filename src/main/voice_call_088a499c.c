/* Calls the device function for the voice's handle. */
typedef struct Mixer088A499C {
    int field_00;
    int field_04;
    void *device;
} Mixer088A499C;

typedef struct Voice088A499C {
    char pad_00[0x10];
    int handle;
} Voice088A499C;

extern Mixer088A499C *D_089D6D80;

void func_08941C04(void *device, int handle);

void func_088A499C(Voice088A499C *v)
{
    int handle = v->handle;
    func_08941C04(D_089D6D80->device, handle);
}
