/* func_088C08BC: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern float func_088C08F0(int param_1,int param_2,int param_3);

int func_088C08BC(int param_1,int param_2)

{
  float fVar1;
  
  fVar1 = (float)func_088C08F0((int)*(short *)(param_1 + 4),(int)*(short *)(param_1 + 6),param_2);
  return (int)fVar1 * 5;
}
