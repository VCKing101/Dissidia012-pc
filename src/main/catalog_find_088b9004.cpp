/* Binary-searches the catalog's second list for an id. */
#include "main/agent17_types.h"

extern "C" {

CatalogWord *func_088B7204(CatalogWord *begin, CatalogWord *end, const CatalogWord *key, SearchOrder order);

void func_088B9004(Catalog *catalog, int id)
{
    CatalogWord key;
    key.id = id;
    func_088B7204(catalog->second, catalog->second + catalog->second_count, &key, SearchOrder());
}

}
