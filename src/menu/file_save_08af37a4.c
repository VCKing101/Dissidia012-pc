/* Prepares the save data, then writes it to the save file (0x120 bytes). */
extern char D_08B0002A[];
extern void func_08AF3824(void *self, void *file, void *data, unsigned char flag);
extern void func_088D3B88(void *file, void *data, void *name, int zero, int size);

void func_08AF37A4(void *self, void *file, void *data, unsigned char flag)
{
    func_08AF3824(self, file, data, flag);
    func_088D3B88(file, data, D_08B0002A, 0, 0x120);
}
