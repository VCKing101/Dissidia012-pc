/* Returns the entry's second value for kind 2, otherwise its first. */
typedef struct Entry088AF720 {
    int field_00;
    int field_04;
    int first;
    int second;
} Entry088AF720;

extern Entry088AF720 D_089D7140[];

int func_088AF720(int unused, int index, int kind)
{
    if (kind == 2) {
        return D_089D7140[index].second;
    }
    return D_089D7140[index].first;
}
