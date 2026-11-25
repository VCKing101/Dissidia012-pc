typedef struct Z { char pad[0x360]; short a0, a1; int w; short b0, b1; int x; short c0, c1; } Z;
void func_088E39D4(int *s, int a, unsigned char c)
{
    s[0] = a;
    if (c == 0) {
        s[1] = 0;
    }
    { int i; int *p;
    for (i = 0, p = s + 0x58; i < 0x40; i++, p += 2) {
        *(short *)p = -1;
        *(short *)((char *)p + 2) = -1;
        p[1] = 0;
    }
    }
    ((Z *)s)->a0 = -1;
    ((Z *)s)->a1 = -1;
    s[0xd9] = 0;
    ((Z *)s)->b0 = -1;
    ((Z *)s)->b1 = -1;
    s[0xdb] = 0;
    ((Z *)s)->c0 = -1;
    ((Z *)s)->c1 = -1;
    s[0xdd] = 0;
    { int i; int *p;
    for (i = 0, p = s; i < 3; i++, p++) {
        p[0xde] = -1;
        p[0xe1] = -1;
    }
    }
    { int i; int *p; int *q; int j;
    for (i = 0, p = s, q = s; i < 100; i++, p++, q = (int *)((char *)q + 2)) {
        p[0xe5] = 0;
        *(short *)((char *)q + 0x524) = -1;
    }
    }
    { int i; int *p;
    for (i = 0, p = s; i < 4; i++, p++) {
        p[0x17b] = 0;
    }
    }
    s[0x19f] = 0;
    s[0x1a0] = 0;
    { int i; int *p;
    for (i = 0, p = s; i < 2; i++, p++) {
        p[0x17f] = 0;
    }
    }
    { int i; int *p; int *q; int j;
    for (i = 0, p = s, q = s; i < 10; i++, p++, q = (int *)((char *)q + 2)) {
        p[0x181] = 0;
        *(short *)((char *)q + 0x644) = -1;
    }
    }
    { int i; int *p;
    for (i = 0, p = s; i < 4; i++, p++) {
        p[0x18b] = 0;
    }
    }
    { int i; int *p;
    for (i = 0, p = s; i < 2; i++, p++) {
        p[399] = 0;
    }
    }
    s[0x196] = 0;
    { int i; int *p;
    for (i = 0, p = s; i < 4; i++, p++) {
        p[0x197] = 0;
    }
    }
    s[0x1a1] = 0;
    s[0x1a2] = 0;
    s[0x1a3] = 0;
    s[0x1a4] = 0;
    if (c != 0) return;
    { int i; int *p;
    for (i = 0, p = s; i < 21; i++, p++) {
        p[2] = 0;
    }
    }
    s[0x17] = 0;
    s[0xe4] = 0;
    s[0x19b] = 0;
    s[0x19c] = 0;
    s[0x19d] = 0x100000;
    s[0x19e] = 0;
    { int i; int *p;
    for (i = 0, p = s; i < 64; i++, p++) {
        p[0x18] = 0;
    }
    }
    s[0x1a5] = 0;
    s[0x1a6] = 0;
    s[0x1a7] = 0;
    s[0x1a8] = 0;
    s[0x1a9] = 0;
    { int i; int *p;
    for (i = 0, p = s + 0x1aa; i < 4; i++, p += 0x11) {
        int *b = p;
        int j;
        int *q;
        for (j = 0, q = b; j < 8; j++, q++) {
            *q = 0;
            q[8] = 0;
        }
        *((char *)b + 0x40) = 0;
        *((char *)b + 0x41) = 0;
        *((char *)b + 0x43) = 0;
        *((char *)b + 0x42) = 0;
    }
    }
    s[0x1ee] = -1;
}
