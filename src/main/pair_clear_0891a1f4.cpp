extern "C" {
typedef struct WordPair { int values[2]; } WordPair;
void func_0891A1F4(WordPair *pair)
{
    for (int i = 0; i < 2; i++) {
        int *v = &pair->values[i];
        *v = 0;
    }
}
}
