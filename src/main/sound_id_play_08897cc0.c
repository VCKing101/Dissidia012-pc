/* Plays a packed sound id: bits 9-15 pick one of four sound groups, bits
   0-8 the sound within it. */
extern void func_08897B3C(void *self, int index, unsigned char flag);
extern void func_08897BFC(void *self, int index, unsigned char flag);
extern void func_08897B9C(void *self, int index, unsigned char flag);
extern void func_08897C5C(void *self, int index, unsigned char flag);

void func_08897CC0(void *self, int id, unsigned char flag)
{
    int group = id >> 9 & 0x7F;
    int index = id & 0x1FF;

    switch (group) {
    case 0:
        func_08897B3C(self, index, flag);
        break;
    case 1:
        func_08897BFC(self, index, flag);
        break;
    case 2:
        func_08897B9C(self, index, flag);
        break;
    case 3:
        func_08897C5C(self, index, flag);
        break;
    }
}
