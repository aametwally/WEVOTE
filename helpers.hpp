#include "headers.hpp"
  
extern map<uint32_t, uint32_t> parentMap;
extern map<uint32_t, string> rankMap;
extern map<uint32_t, uint32_t> standardMap;
extern map<uint32_t, string> namesMap;  
extern map<string, uint32_t> namesTaxMap;  
extern uint32_t undefined;  

uint32_t correctTaxan(uint32_t tempTax);
void correctTaxa(vector<readsInfo> &seq);
uint32_t lca(uint32_t a, uint32_t b);
set<uint32_t> get_ancestry(uint32_t taxon);
uint32_t resolve_tree(map<uint32_t, uint32_t> &hit_counts, uint32_t numToolsReported, uint32_t minNumAgreed);
map<string, uint32_t> build_name_map_taxid(string filename);
uint32_t getTaxonID(string name);


void correctTaxa_vecTaxa(vector<uint32_t> &InputTaxa);
