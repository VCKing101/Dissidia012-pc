/* Flushes the sound queue while sound is on; clears the busy flag when nothing is playing. */
extern unsigned char D_089D69DC;
extern int D_089D69D4;
extern unsigned char D_089D69DD;
extern char D_09B49128[];

void func_08883D90(void *queue);

void func_08883850(void)
{
    if (D_089D69DC == 0) {
        return;
    }
    func_08883D90(D_09B49128);
    if (D_089D69D4 != 0) {
        return;
    }
    D_089D69DD = 0;
}
