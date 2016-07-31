#include "create_db.hpp"


/// Build parent map: take taxon, get parent taxon
map<uint32_t, uint32_t> build_full_taxid_map(string filename) 
{
	map<uint32_t, uint32_t> pmap;
	uint32_t node_id, parent_id;
	string line;
	ifstream ifs(filename.c_str());
	if (ifs.rdstate() & ifstream::failbit) 
	{
		err(EX_NOINPUT, "error opening %s", filename.c_str());
	}

	while (ifs.good()) 
	{
		getline(ifs, line);
		
		if (line.empty())
			break;
		
		sscanf(line.c_str(), "%d\t|\t%d", &node_id, &parent_id);
		pmap[node_id] = parent_id;
	}
	pmap[1] = 0;
	return pmap;
}


/// Build rank map: take taxID, get rank
map<uint32_t, string> build_full_rank_map(string filename) 
{
	map<uint32_t, string> pmap;
	uint32_t node_id, parent_id;
	char rank[100];
	string line;
	ifstream ifs(filename.c_str());
	if (ifs.rdstate() & ifstream::failbit) 
	{
		err(EX_NOINPUT, "error opening %s", filename.c_str());
	}
	while (ifs.good()) 
	{
		getline(ifs, line);
		
		if (line.empty())
			break;
		
		sscanf(line.c_str(), "%d\t|\t%d\t|\t%s", &node_id, &parent_id, rank);
		pmap[node_id] = rank;
	}
	pmap[1] = "root";
	return pmap;
}



/// Build standard parent map: take taxon, get standard parent taxon
map<uint32_t, uint32_t> build_standard_taxid_map(string filename, map<uint32_t, uint32_t> parentMap, map<uint32_t, string> rankMap) 
{
	map<uint32_t, uint32_t> pmap;
	uint32_t node_id, parent_id;
	char rank[100];
	string rankString;
	string line, tempRank;
	ifstream ifs(filename.c_str());
	if (ifs.rdstate() & ifstream::failbit) 
	{
		err(EX_NOINPUT, "error opening %s", filename.c_str());
	}

	while (ifs.good()) 
	{
		getline(ifs, line);
		if (line.empty())
			break;
		sscanf(line.c_str(), "%d\t|\t%d\t|\t%s", &node_id, &parent_id, rank);
		rankString=rank;

		if(rankString=="superkingdom" || rankString=="phylum" || rankString=="class" || rankString=="order" || rankString=="family" || rankString=="genus" || rankString=="species")
		{
			while(1)
			{
				tempRank=rankMap[parent_id];
				if(tempRank=="root" || tempRank=="superkingdom" || tempRank=="phylum" || tempRank=="class" || tempRank=="order" || tempRank=="family" || tempRank=="genus" || tempRank=="species")
				{
					pmap[node_id] = parent_id;
					break;
				}			  
				parent_id=parentMap[parent_id];		  
			}
		}
	}
	pmap[1] = 0;
	return pmap;
}




/// Build name map: take taxon, get name
map<uint32_t, string> build_taxname_map(string filename) 
{
	map<uint32_t, string> pmap;
	uint32_t node_id;
	char name[1000];
	string line;
	ifstream ifs(filename.c_str());
	if (ifs.rdstate() & ifstream::failbit) 
	{
		err(EX_NOINPUT, "error opening %s", filename.c_str());
	}
	while (ifs.good()) 
	{
		getline(ifs, line);
		if (line.empty())
			break;
		sscanf(line.c_str(), "%d\t|\t%s", &node_id, name);
		pmap[node_id] = name;
	}
	return pmap;
}


/// Build reverse name map: take name, get taxon
map<string, uint32_t> build_name_map_taxid(string filename) 
{
	map<string, uint32_t> pmap;
	uint32_t node_id;
	char name[1000];
	string line;
	ifstream ifs(filename.c_str());
	if (ifs.rdstate() & ifstream::failbit) 
	{
		err(EX_NOINPUT, "error opening %s", filename.c_str());
	}
	while (ifs.good()) 
	{
		getline(ifs, line);
		if (line.empty())
			break;
		sscanf(line.c_str(), "%d\t|\t%s", &node_id, name);
		pmap[name] = node_id;
	}
	return pmap;
}
