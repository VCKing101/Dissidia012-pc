/* Fills the menu command record at (char *)self->rec: copies a 13-word header and an optional 300-byte payload, then picks the next state. */
typedef struct H2B824 { int w[13]; } H2B824;
typedef struct P2B824 { int w[75]; } P2B824;
typedef struct R2B824 { int w[1]; } R2B824;
typedef struct S2B824 { char pad[0xA14]; int w14; R2B824 *rec; } S2B824;
void func_08B2B824(S2B824 *self, int a, H2B824 *src, P2B824 *pay, int mode, int dflt, int extra)
{
    self->w14 = a;
    *(H2B824 *)((char *)self->rec + 0x70C) = *src;
    *(int *)((char *)self->rec + 0x18) = *(int *)((char *)self->rec + 0x70C);
    *(unsigned char *)((char *)self->rec + 0x16) = *(unsigned char *)((char *)self->rec + 0x710);
    *(unsigned short *)((char *)self->rec + 0x14) = *(unsigned short *)((char *)self->rec + 0x712);
    *(int *)((char *)self->rec + 0xC) = mode;
    *(int *)((char *)self->rec + 0x708) = 0;
    *(int *)((char *)self->rec + 0x704) = 0;
    *(int *)((char *)self->rec + 0xA0C) = 0;
    *(int *)((char *)self->rec + 0xA10) = extra;
    if (pay == 0) {
        *(int *)((char *)self->rec + 0x740) = dflt;
        *(int *)((char *)self->rec + 4) = 1;
    } else {
        *(P2B824 *)((char *)self->rec + 0x8DC) = *pay;
        if (*(int *)((char *)self->rec + 0xC) == 4) {
            *(int *)((char *)self->rec + 0x740) = 0;
            *(int *)((char *)self->rec + 4) = 1;
        } else {
            *(int *)((char *)self->rec + 0x740) = *(int *)((char *)self->rec + 0x8E0);
            *(int *)((char *)self->rec + 4) = 9;
        }
    }
}
