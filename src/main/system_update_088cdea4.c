/* Updates the global player system when it exists. */
extern int D_089D7810;

void func_088CDED8(int system);

void func_088CDEA4(void)
{
    if (D_089D7810 == 0) {
        return;
    }
    func_088CDED8(D_089D7810);
}
