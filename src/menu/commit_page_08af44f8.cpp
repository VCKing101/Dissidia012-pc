// Stores the current mode into the owning record (local or shared), refreshes the page and registers its widgets.
extern "C" {
int func_08AF9F1C(void *self);
void func_08AF1434(void *self);
int func_08AF96B0(void);
void func_088D428C(char *a, char *b, const char *c, int d);
void func_08AF3ECC(void *self);
void func_08AF1BDC(void *self);
extern const char D_08B005D6[];
extern const char D_08B00646[];
}

struct Shared44F8 {
    char pad[0x1C];
    unsigned flags;
    char pad20[0x24];
    char bytes[1];
    bool Has(unsigned m) { return (flags & m) != 0; }
};
struct Page44F8 { char pad[0x140]; char *blk; };
extern "C" {
extern Shared44F8 *D_089D8140;
struct Blk44F8 { char pad[8]; int idx; int arr[1]; char pad10[0xD0]; int mode; };
extern Blk44F8 D_09B6CDA0;
}

extern "C" void func_08AF44F8(Page44F8 *self)
{
    if (D_089D8140->Has(1)) {
        *(char *)((char *)D_089D8140 + D_09B6CDA0.arr[D_09B6CDA0.idx] + 0x44) = (char)D_09B6CDA0.mode;
    } else {
        if (func_08AF9F1C(self) == 0) {
            *(char *)(self->blk + 0x13E4) = (char)D_09B6CDA0.mode;
        }
    }
    func_08AF1434(self);
    {
        int idle = func_08AF96B0();
        char *a = self->blk + 0x41C;
        char *b = self->blk + 0x8A8;
        if (idle != 0) {
            func_088D428C(a, b, D_08B00646, 1);
        } else {
            func_088D428C(a, b, D_08B005D6, 1);
        }
    }
    func_08AF3ECC(self);
    func_08AF1BDC(self);
}
