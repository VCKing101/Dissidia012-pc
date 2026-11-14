/* func_089B2C5C: generated from provisional Ghidra pseudocode by tools/dc_auto.py;
   byte-identical with Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile). Names are placeholders. */

int func_089B2C5C(unsigned int *param_1,unsigned int *param_2,int *param_3)

{
  unsigned int uVar1;
  unsigned int uVar2;
  
  uVar1 = *param_1;
  uVar2 = param_1[2];
  if (param_2 != (unsigned int *)0x0) {
    if (uVar2 < uVar1) {
      return 0;
    }
    *param_2 = uVar1;
  }
  if (param_3 != (int *)0x0) {
    *param_3 = uVar2 - uVar1;
  }
  return uVar1 - param_1[1];
}
