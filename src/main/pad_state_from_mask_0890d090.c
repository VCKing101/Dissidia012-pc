/* Converts a button bit mask into the pad state record (button flags and the two stick bytes). */
typedef struct Pad { int a; int f; unsigned char x; unsigned char y; } Pad;
void func_0890D090(int unused, Pad *p, unsigned short m)
{
    p->f = 0;
    p->x = 0x80;
    p->y = 0x80;
    p->a = 0;
    if (m & 0x1) p->f |= 0x80;
    if (m & 0x2) p->f |= 0x20;
    if (m & 0x4) p->f |= 0x10;
    if (m & 0x8) p->f |= 0x40;
    if (m & 0x10) p->f |= 0x2000;
    if (m & 0x20) p->f |= 0x4000;
    if (m & 0x40) p->f |= 0x8000;
    if (m & 0x80) p->f |= 0x1000;
    if (m & 0x100) p->f |= 0x100;
    if (m & 0x200) p->f |= 0x200;
    if (m & 0x400) p->f |= 0x8;
    if (m & 0x800) p->f |= 0x1;
    if (m & 0x1000) p->x = 0x0;
    if (m & 0x2000) p->x = 0xFF;
    if (m & 0x4000) p->y = 0x0;
    if (m & 0x8000) p->y = 0xFF;
}
