typedef struct SdkLink {
    int f0;
    struct SdkLink *next;
} SdkLink;

void func_089B126C(SdkLink *list, SdkLink *node)
{
    if (list != 0) {
        if (node != 0) {
            SdkLink **pp = &list->next;
            SdkLink *q;
            while ((q = *pp) != 0) {
                pp = &q->next;
            }
            *pp = node;
        }
    }
}
