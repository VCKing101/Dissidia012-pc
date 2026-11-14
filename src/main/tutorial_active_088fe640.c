/* True while tutorials are on and either forced or allowed by func_08883340. */
extern unsigned char D_089D7F9C;
extern unsigned char D_089D69DC;
extern int func_08883340(void);

int func_088FE640(void)
{
    if (D_089D7F9C == 0) {
        return 0;
    }
    if (D_089D69DC == 0) {
        int allowed = func_08883340();

        if (allowed == 0) {
            return 0;
        }
    }
    return 1;
}
