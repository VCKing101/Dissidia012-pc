/* Draws a weighted pick among six options (weights from func_088B6BE0, roll 0-99 from the global generator) and applies it with func_088F145C. */
extern int D_09AFB89C;
int func_088B6BE0(int i);
void func_088F145C(int i);
void func_088F049C(void)
{
    int roll;
    int acc;
    int pick;
    int i;
    int w;
    D_09AFB89C = D_09AFB89C * 0x343FD + 0x269EC3;
    roll = (D_09AFB89C >> 16 & 0x7FFF) % 100;
    acc = 0;
    pick = 0;
    for (i = 0; i < 6; i++) {
        w = func_088B6BE0(i);
        if (w == 0) {
            continue;
        }
        acc += w;
        if (roll < acc) {
            pick = i;
            break;
        } else {
            continue;
        }
    }
    func_088F145C(pick);
}
