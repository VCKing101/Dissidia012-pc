/* Releases all 256 sounds of the bank, last first. */
void func_088DAB44(int sound);
void func_088D3A04(int *sounds)
{
    int i;
    for (i = 255; i >= 0; i--) {
        if (sounds[i] != 0) {
            if (sounds[i] != 0) {
                func_088DAB44(sounds[i]);
                sounds[i] = 0;
            }
        }
    }
}
