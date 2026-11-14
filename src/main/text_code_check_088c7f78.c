/* 1 when the code is valid (not -1) and has bit 15 set. */
unsigned char func_088C7F78(unsigned int code)
{
    int result = 0;
    if (code != 0xFFFFFFFF && (code & 0x8000)) {
        result = 1;
    }
    return result;
}
