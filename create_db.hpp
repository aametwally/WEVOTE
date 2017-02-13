#include "headers.hpp"

extern map<uint32_t, uint32_t> parentMap;
extern map<uint32_t, string> rankMap;
extern map<uint32_t, uint32_t> standardMap;
extern map<uint32_t, string> namesMap;


map<uint32_t, uint32_t> build_full_taxid_map(string filename);
map<uint32_t, string> build_full_rank_map(string filename);
map<uint32_t, uint32_t> build_standard_taxid_map(string filename, map<uint32_t, uint32_t> parentMap, map<uint32_t, string> rankMap);
map<uint32_t, string> build_taxname_map(string filename);
