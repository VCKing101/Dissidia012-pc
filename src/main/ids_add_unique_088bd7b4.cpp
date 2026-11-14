struct Source { short id; short pad; short *flag; };
extern "C" {
int *func_088BDCC0(int *begin, int *end, const int *key);

void func_088BD7B4(int *ids, int max, int *count, Source *src)
{
    for (int i = 0; i < 4; i++, src++) {
        int id = src->id;
        if (*src->flag != 0) {
            if (func_088BDCC0(ids, ids + *count, &id) == ids + *count) {
                if (*count >= max) {
                    return;
                }
                ids[*count] = id;
                (*count)++;
            }
        }
    }
}
}
