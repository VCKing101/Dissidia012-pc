/* Percentage of the five categories' cleared counts against the total. */
extern int func_08A29660(void);
extern int func_08A29840(int category);

int func_08A2ACA0(void)
{
    int total = func_08A29660();
    int cleared = func_08A29840(0);
    cleared += func_08A29840(1);
    cleared += func_08A29840(2);
    cleared += func_08A29840(3);
    cleared += func_08A29840(4);
    return cleared * 100 / total;
}
