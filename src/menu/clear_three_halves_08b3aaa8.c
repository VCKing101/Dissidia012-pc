/* Zeroes three halfwords at 0x9A..0x9E of the block at 0x09B6DAC0. */
struct H { char pad[0x9A]; short a, b, c; };
extern struct H D_09B6DAC0;
void func_08B3AAA8(void) { D_09B6DAC0.a = 0; D_09B6DAC0.b = 0; D_09B6DAC0.c = 0; }
