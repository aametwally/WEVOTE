#include "helpers.hpp"


/// Convert non-standard taxon to a standard taxon
void correctTaxa(vector<readsInfo> &seq)
{
	uint32_t tempTax;
	string tempRank="";
		
	for (uint32_t i=0 ; i<seq.size() ;i++)
	{
	
		for (uint32_t j=0 ; j<seq[i].annotation.size(); j++)
		{	
		
			tempTax = seq[i].annotation[j];
			tempRank = rankMap[tempTax];
					
			if (tempTax==0)
			{
				continue;
			}		
			else if (tempRank == "")
			{
				undefined++;
				seq[i].annotation[j] =0;
				cout << "The taxonomy on line= " << i << "  which is= " << seq[i].annotation[j] << " is not availbe \n";
				continue;			
			}

			if (!( tempRank=="root" || tempRank == "superkingdom" || tempRank== "phylum" || tempRank == "class" || tempRank == "order" || tempRank == "family" || tempRank == "genus" || tempRank == "species"))	
			{	
				while(1)
				{				
					tempTax=parentMap[tempTax];
					tempRank=rankMap[tempTax];
					if(tempRank=="root" || tempRank=="superkingdom" || tempRank=="phylum" || tempRank=="class" || tempRank=="order" || tempRank=="family" || tempRank=="genus" || tempRank=="species")
					{
						seq[i].annotation[j] = tempTax;
						break;
					}						  
				}
			}
		}	
	}
}



/// For an array of taxa: Convert non-standard taxon to a standard taxon
void correctTaxa_vecTaxa(vector<uint32_t> &InputTaxa)
{
	uint32_t tempTax;
	string tempRank="";
		
	for (uint32_t i=0 ; i<InputTaxa.size() ;i++)
	{
		
		
			tempTax = InputTaxa[i];
			tempRank = rankMap[tempTax];
					
			if (tempTax==0)
			{
				continue;
			}		
			else if (tempRank == "")
			{
				undefined++;
				InputTaxa[i] =0;
				cout << "The taxonomy on line= " << i << "  which is= " << InputTaxa[i] << " is not availbe \n";
				continue;
			}

			if (!( tempRank=="root" || tempRank == "superkingdom" || tempRank== "phylum" || tempRank == "class" || tempRank == "order" || tempRank == "family" || tempRank == "genus" || tempRank == "species"))	
			{	
				while(1)
				{				
					tempTax=parentMap[tempTax];
					tempRank=rankMap[tempTax];
					if(tempRank=="root" || tempRank=="superkingdom" || tempRank=="phylum" || tempRank=="class" || tempRank=="order" || tempRank=="family" || tempRank=="genus" || tempRank=="species")
					{
						InputTaxa[i] = tempTax;
						break;
					}						  
				}
			}			
	}
}


/// Get LCA
uint32_t lca(uint32_t a, uint32_t b)
{
	if (a == 0 || b == 0)
		return a ? a : b;
	
	set<uint32_t> a_path;
	while (a > 0) 
	{
		a_path.insert(a);
		a = standardMap[a];
	}
	while (b > 0) 
	{
		if (a_path.count(b) > 0)
			return b;
		b = standardMap[b];
	}
	return 1;
}



/// Get all ancestry of a taxon
set<uint32_t> get_ancestry(uint32_t taxon)
{
	set<uint32_t> path;
	path.clear();
	while (taxon > 0) 
	{
		path.insert(taxon);
		taxon = standardMap[taxon];
	}
	return path;
}


/// Return the taxon of the highest weighted Root-to-Taxon path and passes WEVOTE threshold.
uint32_t resolve_tree(map<uint32_t, uint32_t> &hit_counts, uint32_t numToolsReported, int minNumAgreed)
{
	set<uint32_t> max_taxa;
	uint32_t max_taxon = 0, max_score = 0;
	map<uint32_t, uint32_t>::iterator it = hit_counts.begin();	
	int Threshold=floor(0.5*(double)numToolsReported);
	
	if((minNumAgreed-1)>Threshold)
	Threshold=(minNumAgreed-1);

	while (it != hit_counts.end()) 
	{
		if(it->second <=Threshold)
		{
			it++;
			continue;
		}
		
		uint32_t taxon = it->first;
		uint32_t node = taxon;
		uint32_t score = 0;
		while (node > 0) 
		{
			score += hit_counts[node];
			node = standardMap[node];
		}

		if (score > max_score) 
		{
			max_taxa.clear();
			max_score = score;
			max_taxon = taxon;
		}
		else if (score == max_score) 
		{
			if (max_taxa.empty())
				max_taxa.insert(max_taxon);
			
			max_taxa.insert(taxon);
		}

		it++;
	}

	if (! max_taxa.empty()) 
	{
		set<uint32_t>::iterator sit = max_taxa.begin();
		max_taxon = *sit;
		
		for (sit++; sit != max_taxa.end(); sit++)
		max_taxon = lca(max_taxon, *sit);
	}
	return max_taxon;
}

