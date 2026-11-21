/* gu_format_bits: bits per pixel of a GE pixel format (GU_PSM_*); 0 for an unknown one. */
int func_08973EC8(unsigned int format)
{
    int bits = 0;

    switch (format) {
    case 0:                            /* GU_PSM_5650 */
        bits = 16;
        break;
    case 1:                            /* GU_PSM_5551 */
        bits = 16;
        break;
    case 2:                            /* GU_PSM_4444 */
        bits = 16;
        break;
    case 3:                            /* GU_PSM_8888 */
        bits = 32;
        break;
    case 4:                            /* GU_PSM_T4 */
        bits = 4;
        break;
    case 5:                            /* GU_PSM_T8 */
        bits = 8;
        break;
    case 6:                            /* GU_PSM_T16 */
        bits = 16;
        break;
    case 7:                            /* GU_PSM_T32 */
        bits = 32;
        break;
    }
    return bits;
}
