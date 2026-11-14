/* Prepares a query from the last two arguments and runs it when that succeeds; 0 otherwise. */
typedef struct Query088C1C50 {
    int a;
    int b;
    int c;
} Query088C1C50;

int func_088C1CFC(int a, Query088C1C50 *query, int d, int e);
int func_088C1F6C(int a, int b, int c, Query088C1C50 *query, int e);
int func_088C1C50(int a, int b, int c, int d, int e)
{
    Query088C1C50 query;
    if (func_088C1CFC(a, &query, d, e) == 1) {
        return func_088C1F6C(a, b, c, &query, e);
    }
    return 0;
}
