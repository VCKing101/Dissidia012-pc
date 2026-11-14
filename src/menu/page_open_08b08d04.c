/* Shows the page title and icons, opens the list or the empty view and
   applies the page. */
struct Title08B08D04 {
    char pad_0[0xE];
    unsigned short field_E;
};

struct State08B08D04 {
    char pad_00000[0x4F4];
    struct Title08B08D04 *field_4F4;
    char pad_004F8[0x688 - 0x4F8];
    void *field_688;
    char pad_0068C[0x1F918 - 0x68C];
    int field_1F918;
    char pad_1F91C[0x1F924 - 0x1F91C];
    int field_1F924;
};

struct Owner08B08D04 {
    char pad_000[0x138];
    struct State08B08D04 *field_138;
};

extern unsigned short D_08B1CA74[];
extern void func_088D9F5C(void *part, int id, int show);
extern int func_08B06BA0(struct Owner08B08D04 *self);
extern void func_08B06FDC(struct Owner08B08D04 *self);
extern void func_08B069A8(struct Owner08B08D04 *self);
extern void func_08B06070(struct Owner08B08D04 *self, unsigned char force);

void func_08B08D04(struct Owner08B08D04 *self)
{
    func_088D9F5C(self->field_138->field_4F4,
                  D_08B1CA74[self->field_138->field_1F918 - self->field_138->field_1F924], 1);
    self->field_138->field_4F4->field_E = 0x22;
    func_088D9F5C(self->field_138->field_688, 4, 1);
    if (func_08B06BA0(self) != 0) {
        func_08B06FDC(self);
    } else {
        func_08B069A8(self);
    }
    func_08B06070(self, 0);
}
