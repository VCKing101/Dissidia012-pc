/* Computes two angle constants (45 and 135 degrees) from a base value divided by 180. */
extern float D_08B3ABB8;
extern float D_08B3AD90;
extern float D_08B3AD94;
void func_08B3A968(void) {
    D_08B3AD90 = D_08B3ABB8 / 180.0f * 45.0f;
    D_08B3AD94 = D_08B3ABB8 / 180.0f * 135.0f;
}
