/* func_08908FAC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08908FDC(int param_1,int param_2);

void func_08908FAC(int param_1)

{
  if (*(unsigned char *)(param_1 + 0x25c) == '\x01') {
    *(unsigned char *)(param_1 + 0x25c) = 0;
    func_08908FDC(param_1,4);
  }
  return;
}
