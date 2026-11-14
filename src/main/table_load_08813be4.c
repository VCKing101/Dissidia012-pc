typedef struct Agent4Resource {
    int field_00;
    int field_04;
    int handle;
} Agent4Resource;

typedef struct Agent4Manager {
    unsigned char unknown_0000[0x500];
    unsigned char field_0500[4];
} Agent4Manager;

extern Agent4Manager *D_089D4DC8;

Agent4Resource *func_0885CA98(int owner, int type, int id, int arg);
void func_08870B10(int handle, int *out0, int *out1);
void func_0885911C(void *part, int arg, int value0, int value1);

void func_08813BE4(int owner, signed char *name, int arg, int target) {
    int value0 = 0;
    int value1 = 0;
    Agent4Resource *res = func_0885CA98(owner, 3, name[0] | (name[1] << 8) | (name[2] << 16) | (name[3] << 24), arg);
    if (res != 0) {
        func_08870B10(res->handle, &value0, &value1);
        {
            int second = value1;
            int first = value0;
            func_0885911C(D_089D4DC8->field_0500, target, first, second);
        }
    }
}
