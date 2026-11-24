/* Clears a flag byte in D_09B6CDA0 and stores func_08AF4240 of its buffer (0x23, 0) into +0xA58. */
typedef struct { char pad0[0x9BC]; unsigned char flag; char pad1[0x9CC - 0x9BD]; char buf[0x23]; char pad2[0xA58 - 0x9CC - 0x23 + 0]; int result; } Obj;
extern Obj D_09B6CDA0;
extern int func_08AF4240(void *a, void *b, int n, int z);
void func_08AF7E90(void *a) {
    D_09B6CDA0.flag = 0;
    D_09B6CDA0.result = func_08AF4240(a, D_09B6CDA0.buf, 0x23, 0);
}
