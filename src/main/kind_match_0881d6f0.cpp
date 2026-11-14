typedef struct Info0881D6F0 {
    short id;
    unsigned char unknown_02[0x12];
    short count;
} Info0881D6F0;

typedef struct Owner0881D6F0 {
    unsigned char unknown_00[0x48];
    Info0881D6F0 *info;
} Owner0881D6F0;

extern "C" int D_089D4910;
extern "C" int func_0881E7E0(int table, Info0881D6F0 *info, int index);

extern "C" int func_0881D6F0(void *self, Owner0881D6F0 *owner, int id) {
    Info0881D6F0 *info;
    if (owner == 0) {
        return 0;
    }
    info = owner->info;
    if (info == 0) {
        return 0;
    }
    if (info->id == id) {
        return 1;
    }
    for (int i = 0; i < info->count; i++) {
        if (func_0881E7E0(D_089D4910, info, i) == id) {
            return 1;
        }
    }
    return 0;
}
