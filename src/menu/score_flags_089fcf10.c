/* Sets the score flags of the save block in D_089D726C: 4 from a score of
   70, 0x100 from 100. */
typedef struct Progress29 {
    char pad_00[0x4];
    char flags[4];  /* +0x04, a 32-bit set */
} Progress29;

extern Progress29 *D_089D726C;
extern int func_089FCD38(void);

void func_089FCF10(void)
{
    int score = func_089FCD38();

    if (score >= 0x46) {
        *(unsigned int *)D_089D726C->flags |= 4;
    }
    if (score >= 100) {
        *(unsigned int *)D_089D726C->flags |= 0x100;
    }
}
