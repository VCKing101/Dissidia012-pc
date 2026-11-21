typedef struct Item088849A4 {
    void *sprite;                      /* +0x00 */
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} Item088849A4;

extern void *D_089D7830;               /* the texture manager */
extern void func_08885B0C(Item088849A4 *self);
extern void *func_088D0700(void *mgr, unsigned char group, unsigned short id, unsigned short flags);

Item088849A4 *func_088849A4(Item088849A4 *self, unsigned char group, unsigned short id, unsigned short flags)
{
    func_08885B0C(self);
    if (id != 0xFFFF) {
        self->sprite = func_088D0700(D_089D7830, group & 0xFF, id, flags);
        self->unk_08 = 0;
    } else {
        self->sprite = 0;
        self->unk_08 = 0;
    }
    self->unk_04 = 0;
    self->unk_0C = 0;
    self->unk_10 = 0;
    self->unk_14 = 0;
    return self;
}
