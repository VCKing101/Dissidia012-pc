/* func_088A98B4: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_088A9E90(unsigned int param_1,int param_2,unsigned int param_3,unsigned int param_4,
                 unsigned int param_5);

void func_088A98B4(int param_1,int param_2,unsigned int param_3,unsigned int param_4,unsigned int param_5)

{
  if ((*(unsigned char *)(param_1 + 0x24) == '\x01') && (param_2 != 0)) {
    func_088A9E90(param_2,param_1,param_3,param_4,param_5);
  }
  return;
}
