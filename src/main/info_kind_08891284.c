typedef struct Info08891284 {
    int kind;
    unsigned char unknown_04[0x54];
} Info08891284;

typedef struct Entry08891284 {
    unsigned char valid;
} Entry08891284;

int func_088916A4(Entry08891284 *self, Info08891284 *info);

int func_08891284(Entry08891284 *self) {
    Info08891284 info;
    if (self->valid == 0) {
        return 0;
    }
    if (func_088916A4(self, &info) != 0) {
        return 0;
    }
    return info.kind == 0x33;
}
