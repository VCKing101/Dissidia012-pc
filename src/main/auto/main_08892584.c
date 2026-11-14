/* func_08892584: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08866BA8(int param_1);
extern void func_0886ACCC(int param_1,unsigned int param_2,int param_3);
extern void func_0889245C(int param_1,unsigned int param_2);
extern void func_08895388(int param_1);
extern void func_0899FFC8(unsigned int param_1);
extern void func_089A0384(unsigned int param_1);
extern void func_089A03DC(unsigned int param_1);
extern void func_089A0C20(unsigned int param_1);
extern void func_089A0E20(unsigned int param_1,int param_2);
extern void func_089A0E60(unsigned int param_1,int param_2);
extern void func_089A123C(unsigned int param_1,int param_2,int param_3);
extern void func_089A1270(unsigned int param_1,int param_2,int param_3);
extern void func_089A25C0(void);
extern int D_089D5EC0;

void func_08892584(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x74);
  if ((iVar1 != 0) && (*(unsigned char *)(param_1 + 0x6b2) != '\0')) {
    if (*(int *)(param_1 + 0x4d0) == -1) {
      func_089A0384(0x15);
      func_089A123C(1,0x80,0x80);
      func_089A1270(0,0,2);
      func_089A0384(3);
      if (*(unsigned char *)(param_1 + 0x610) != '\0') {
        func_08895388(param_1 + 0x4e0);
      }
      func_0886ACCC(*(unsigned int *)(param_1 + 0x74),param_1 + 0x10,0);
      func_089A03DC(3);
      func_089A03DC(0x15);
      iVar1 = *(int *)(param_1 + 0x74);
    }
    else {
      func_089A03DC(0x15);
      func_08895388(param_1 + *(int *)(param_1 + 0x4d0) * 0x130 + 0x140);
      iVar1 = *(int *)(param_1 + 0x74);
    }
  }
  if (((iVar1 != 0) && (*(unsigned char *)(param_1 + 0x6b2) != '\0')) && (*(int *)(param_1 + 0x4d0) == -1)) {
    func_08866BA8(D_089D5EC0);
    func_089A25C0();
    func_089A0E20(0,1);
    func_089A0E60(0,0);
    func_089A0384(10);
    func_089A03DC(0xb);
    func_089A03DC(0xc);
    func_089A03DC(0xd);
    func_089A03DC(0xe);
    func_089A0C20(0xff808080);
    func_0899FFC8(1);
    func_089A123C(1,0x80,0x80);
    func_089A1270(0,0,2);
    func_089A0384(3);
    func_089A0384(0x15);
    if (*(unsigned char *)(param_1 + 0x6b4) == '\x01') {
      func_0889245C(param_1,2);
    }
    func_089A03DC(0x15);
    func_089A03DC(3);
    func_089A0C20(0xffffffff);
  }
  return;
}
