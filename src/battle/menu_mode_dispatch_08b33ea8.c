// Handles a direction input on the menu-like object by its mode byte.
extern int func_088377C0(void);
extern int func_08B30898(void *);
extern void func_08B33DDC(void *, int);
typedef struct { char d[0x20]; unsigned char mode; } M;
typedef struct { char d[0x101C]; M *m; } O;
int func_08B33EA8(O *self) {
    unsigned char mode;
    if (!func_088377C0() || !func_08B30898(self)) return 0;
    mode = self->m->mode;
    switch (mode) {
    case 1: func_08B33DDC(self, 0); return 1;
    case 2: func_08B33DDC(self, 1); return 1;
    case 3: func_08B33DDC(self, 2); return 1;
    case 4: func_08B33DDC(self, -1); return 1;
    }
    return 0;
}
