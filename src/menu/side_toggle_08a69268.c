typedef struct O { char p0[0x48]; int h48; int h4c; char p1[0x60-0x50]; int h60; int h64; char p2[0x70-0x68]; int h70; int h74; int h78; int h7c; char p3[0x8c-0x80]; int h8c; int h90; char p4[0x98-0x94]; int h98; int h9c; char p5[0xa8-0xa0]; int ha8; char p6[0xd4-0xac]; int hd4; int hd8; } O;
extern void func_088D9F5C(int, int, int);
extern void func_088D9CD8(int);
extern void func_088D9CFC(int);
extern void func_0887B924(int);
extern void func_0887B944(int);
void func_08A69268(O *self) {
    switch (self->ha8) {
    case 0:
        {
            func_088D9F5C(self->h48, 0x10, 0);
            func_088D9F5C(self->h4c, 0xb, 0);
            func_088D9CD8(self->h70);
            func_088D9CFC(self->h74);
            func_088D9F5C(self->h60, 0xd, 0);
            func_088D9F5C(self->h64, 0xe, 0);
            func_088D9CD8(self->h78);
            func_088D9CFC(self->h7c);
            if (self->h8c != 0) func_0887B924(self->h8c);
            if (self->h90 != 0) func_0887B924(self->h90);
            if (self->h98 != 0) func_0887B944(self->h98);
            if (self->h9c != 0) func_0887B944(self->h9c);
        }
        break;
    case 1:
        {
        func_088D9F5C(self->h48, 0x11, 0);
        func_088D9F5C(self->h4c, 10, 0);
        func_088D9CFC(self->h70);
        func_088D9CD8(self->h74);
        func_088D9F5C(self->h60, 0xe, 0);
        func_088D9F5C(self->h64, 0xd, 0);
        func_088D9CFC(self->h78);
        func_088D9CD8(self->h7c);
        if (self->h8c != 0) func_0887B944(self->h8c);
        if (self->h90 != 0) func_0887B944(self->h90);
        if (self->h98 != 0) func_0887B924(self->h98);
        if (self->h9c != 0) func_0887B924(self->h9c);
        }
        break;
    }
    if (self->hd4 == 0) {
        func_088D9F5C(self->h60, 0xf, 0);
        if (self->h8c != 0) func_0887B944(self->h8c);
        if (self->h90 != 0) func_0887B944(self->h90);
    }
    if (self->hd8 == 0) {
        func_088D9F5C(self->h64, 0xf, 0);
        if (self->h98 != 0) func_0887B944(self->h98);
        if (self->h9c != 0) func_0887B944(self->h9c);
    }
}
