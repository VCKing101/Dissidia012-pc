/* Names the replay file and writes it; true when the save reports 0x80010021. */
typedef struct ReplayState {
    char pad_000[0xD34];
    char field_D34[0x40];
} ReplayState;

typedef struct ReplayOwner {
    char pad_000[0x14C];
    ReplayState *field_14C;
} ReplayOwner;

extern char D_08A45610[];
extern char D_08A4562C[];
extern char D_08A45634[];
extern void func_089359A4(char *dst, const char *src);
extern int func_089AA818(int mode);
extern int func_089AA748(const char *format, int a, int b);
extern int func_089AA264(const char *title, const char *name);

int func_08A41DC4(ReplayOwner *self)
{
    func_089359A4(self->field_14C->field_D34, D_08A45610);
    if (func_089AA818(2) == 0) {
        if (func_089AA748(D_08A4562C, 0, -1) == 0) {
            if (func_089AA264(D_08A45634, self->field_14C->field_D34) == (int)0x80010021) {
                return 1;
            }
        }
    }
    return 0;
}
