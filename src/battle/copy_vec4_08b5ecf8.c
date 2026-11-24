/* Copies the four floats at 0x09AF8AB0 to 0x08B5EE30. */
struct Vec4 { float x, y, z, w; };
extern struct Vec4 D_09AF8AB0;
extern struct Vec4 D_08B5EE30;
void func_08B5ECF8(void)
{
    D_08B5EE30 = D_09AF8AB0;
}
