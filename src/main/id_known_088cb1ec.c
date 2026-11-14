/* 1 when the id is known. */
int func_088CB3B4(int id);
int func_088CB1EC(void *self, int id)
{
    int index = func_088CB3B4(id);
    return index >= 0;
}
