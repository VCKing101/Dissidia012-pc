extern float D_089C9F54;

int func_08A22DEC(float time)
{
    int count = (int)(time / D_089C9F54);
    if (time - (float)count != 0.0f) {
        count++;
    }
    return count;
}
