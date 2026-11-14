/* Returns the entry's second value for kind 2, otherwise its first. */
typedef struct Entry088ACEE0 {
    int field_00;
    int field_04;
    int first;
    int second;
} Entry088ACEE0;

extern Entry088ACEE0 D_089D6DE8[];

int func_088ACEE0(int unused, int index, int kind)
{
    if (kind == 2) {
        return D_089D6DE8[index].second;
    }
    return D_089D6DE8[index].first;
}
