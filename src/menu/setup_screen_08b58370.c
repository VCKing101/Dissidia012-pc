/* Initialises the record's state words (mode 7 unless the flag byte is set), registers two sub-blocks and calls the final hook. */
struct Rec { char pad0[0x814]; int s814; char pad1[0xDAC - 0x818]; int sdac; char pad2[0xDD4 - 0xDB0]; int sdd4; char pad3[0x2B80 - 0xDD8]; unsigned char flag; };
struct Mgr { char pad[0x14]; struct Rec *rec; };
extern struct Mgr *D_089D7260;
extern int D_089D712C;
extern void func_088ADE90(int, void *, void *, int);
extern void func_08B5813C(void *, void *);
extern void func_088ADF28(int, void *, int, int);
extern void func_088AFB30(struct Mgr *);
void func_08B58370(void) {
    struct Rec *r = D_089D7260->rec;
    int v;
    r->s814 = 5;
    r->sdd4 = 0;
    v = r->flag ? 0 : 7;
    r->sdac = v;
    func_088ADE90(D_089D712C, (char *)r + 0x864, (char *)r + 0x2A94, 0);
    func_08B5813C((char *)r + 0x864, (char *)r + 0x2A94);
    func_088ADF28(D_089D712C, (char *)r + 0x8E4, 0x88, 0);
    func_088AFB30(D_089D7260);
}
