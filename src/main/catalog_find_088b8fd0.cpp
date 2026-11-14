/* Binary-searches the catalog's first list for an id. */
#include "main/agent17_types.h"

extern "C" {

CatalogWord *func_088B715C(CatalogWord *begin, CatalogWord *end, const CatalogWord *key, SearchOrder order);

void func_088B8FD0(Catalog *catalog, int id)
{
    CatalogWord key;
    key.id = id;
    func_088B715C(catalog->first, catalog->first + catalog->first_count, &key, SearchOrder());
}

}
