/* func_088AC1AC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088AB9F8(int param_1);
extern void func_088AC034(int param_1,unsigned int param_2);
extern unsigned int func_088AC07C(int param_1,unsigned char param_2);
extern void func_088F1B8C(void);

void func_088AC1AC(int param_1,int param_2,unsigned char param_3)

{
  unsigned int uVar1;
  
  if (*(unsigned char *)(param_1 + 0xefd) == '\0') {
    func_088AB9F8(param_1);
    func_088F1B8C();
    if (param_3 == '\0') {
      uVar1 = *(unsigned int *)(param_1 + 0x3f5fc) & 0xfffffffe;
    }
    else {
      uVar1 = *(unsigned int *)(param_1 + 0x3f5fc) | 1;
    }
    *(unsigned int *)(param_1 + 0x3f5fc) = uVar1;
  }
  *(unsigned int *)(param_1 + 0xef8) = *(unsigned int *)(param_1 + 0xef8) & 0xffffffdf;
  *(int *)(param_1 + 0x210) = param_2;
  *(unsigned int *)(param_1 + 0x7a8) = 0;
  if (param_2 == 5) {
    *(unsigned int *)(param_1 + 0xef8) = *(unsigned int *)(param_1 + 0xef8) & 0xfffffffd;
  }
  func_088AC034(param_1,param_2);
  func_088AC07C(param_1,1);
  return;
}
