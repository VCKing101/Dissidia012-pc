/* func_08892198: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with SNC -O2 -g. Names are placeholders. */
extern void func_08893CEC();
extern int * func_08894D34();
extern void func_08993560();
extern int D_089CA758;
extern int D_089CA768;

int func_08892198(int param_1)

{
  func_08894D34();
  *(char **)(param_1 + 0x734) = &D_089CA758;
  *(char **)(param_1 + 0x120) = &D_089CA768;
  *(int *)(param_1 + 0x130) = 0;
  func_08993560(param_1 + 0x140,3,0x130,func_08894D34);
  func_08894D34(param_1 + 0x4e0);
  *(int *)(param_1 + 0x710) = 0;
  *(int *)(param_1 + 0x714) = 0;
  *(int *)(param_1 + 0x6b8) = 0;
  *(int *)(param_1 + 0x6bc) = 0;
  *(int *)(param_1 + 0x6c0) = 0;
  func_08893CEC(param_1);
  *(int *)(param_1 + 0x718) = 0;
  *(int *)(param_1 + 0x71c) = 0;
  *(int *)(param_1 + 0x720) = 0;
  *(int *)(param_1 + 0x724) = 0;
  *(int *)(param_1 + 0x728) = 0;
  *(int *)(param_1 + 0x72c) = 0;
  *(int *)(param_1 + 0x730) = 0;
  return param_1;
}
