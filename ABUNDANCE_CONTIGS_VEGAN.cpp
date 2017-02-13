#include "headers.hpp"
#include "helpers.hpp"
#include "create_db.hpp"


typedef struct {
	uint32_t taxon;
	uint32_t count;
	string root;
	string superkingdom;
	string kingdom;
	string phylum;
        string clas;
        string order;
        string family;
        string genus;
        string species;
	double RA;   
} taxA;


map<uint32_t, uint32_t> parentMap;
map<uint32_t, string> rankMap;
map<uint32_t, uint32_t> standardMap;
map<uint32_t, string> namesMap;
uint32_t undefined=0;
map<uint32_t, taxA> taxonAnnotate_Map;







int main(int argc, char *argv[])
{	
	string nodesFilename="";
	string namesFilename="";
	string query="";
	string prefix="";
	string taxonomyDB="";
	string abundanceMethod="total";
	string analysisMethod="";
	string classificationMethod="";
	

	if (argc<2)
	{
		cout << "less than the required minimum number of options\n";
		exit(1);
	}
	
	
	/// parse commandline arguments
	while (1) 
	{	
		static struct option long_options[] = 
		{			 
			{"i", 1, 0, 0},
			{"p", 1, 0, 0},
			{"d", 1, 0, 0}, 
			{"t", 1, 0, 0},
			{"a", 1, 0, 0},
			{"c", 1, 0, 0} 
		};
		
		int longindex = -1;
		int c = getopt_long_only(argc, argv, "", long_options, &longindex);
		if(c == -1) break; /// Done parsing flags.
		else if(c == '?') 
		{
			/// If the user entered junk, let him know. 
			cerr << "Invalid parameters." << endl;
			exit(1);
		}
		else 
		{
			switch(longindex) 
			{ 
				case 0: query=optarg; break;
				case 1: prefix=optarg; break;
				case 2: taxonomyDB=optarg; break;
				case 3: abundanceMethod=optarg; break;
				case 4: analysisMethod=optarg; break; // contig or reads
				case 5: classificationMethod=optarg; break; // naive or wevote
				default: break; 
			}
		}
	}
	
	
	nodesFilename=taxonomyDB+"/nodes_wevote.dmp";
	namesFilename=taxonomyDB+"/names_wevote.dmp";
	string OutputProfile= prefix + "_Abundance.csv";
	//~ vector<taxon> TaxaInfo;
	//~ taxon OneTaxon;
	//~ vector<uint32_t> InputTaxa;
	//~ vector<uint32_t> TaxaCount;
	//~ TaxaInfo.clear();
	//~ InputTaxa.clear();
	ifstream file (query.c_str());
	string line = "";
	int it=0;


	/// Build taxonomy trees
	parentMap = build_full_taxid_map(nodesFilename);
	rankMap = build_full_rank_map(nodesFilename);
	standardMap=build_standard_taxid_map(nodesFilename, parentMap, rankMap);
	namesMap = build_taxname_map(namesFilename);
	
	/// Read WEVOTE output file 
	if (!file.is_open())
	{
		cout << "Error opening file:" << query  <<"\n";
		exit(1);
	}


	uint32_t taxon_temp=0;
	uint32_t count_temp=0;
	int q=0;
	while (getline(file, line))
	{
		q++;
		stringstream strstr(line);
		string word = "";
		it=0;

			
		while (getline(strstr,word, ',')) 
		{			
			if(it==1)
			{
				taxon_temp = atoi(word.c_str());
				
				if(taxon_temp == 0)
				{
					cout << "taxon 0= " << taxon_temp << "line = " << q << "\n";
				}
				//~ cout << "taxon before= " << taxon_temp << "\n";
				taxon_temp = correctTaxan(taxon_temp);
				//~ cout << "taxon after= " << taxon_temp << "\n";
			}
			else if(it==2)
			{
					count_temp = atoi(word.c_str());
					//~ cout << count_temp << "\n";
					break;
			}
											
			it++;
		}
		
		
		if ( taxonAnnotate_Map.find(taxon_temp) == taxonAnnotate_Map.end() ) {
			taxonAnnotate_Map[taxon_temp].count = count_temp;
			taxonAnnotate_Map[taxon_temp].taxon = taxon_temp;
		} else {
			taxonAnnotate_Map[taxon_temp].count += count_temp;
		}

			
	}
	

	
	for(map<uint32_t, taxA>::iterator it = taxonAnnotate_Map.begin(); it != taxonAnnotate_Map.end(); ++it)
	{
		if(it->first == 0)
		cout << it->first << " =" << it->second.count << "\n";
	}
	
	
	
	
	//~ uint32_t numSeq= InputTaxa.size();
	//~ uint32_t numClassifiedReads=0;	
	map<uint32_t, uint32_t> hit_counts;
	set<uint32_t> parents;
	std::set<uint32_t>::iterator set_iterator;
	std::map<uint32_t, uint32_t>::iterator iterator_name; 	
	hit_counts.clear();
	uint32_t TotalCounts=0;


	//~ for (uint32_t i=0; i<TaxaCount.size(); i++)
	//~ {
		//~ TotalCounts+=TaxaCount[i];	
	//~ }
		
	for(map<uint32_t, taxA>::iterator it = taxonAnnotate_Map.begin(); it != taxonAnnotate_Map.end(); ++it)
	{		
		TotalCounts += it->second.count;	
	}	
	
	cout << "Total counts = " << TotalCounts << "\n";
	
	
	string rank_temp = "null";
	for(map<uint32_t, taxA>::iterator it = taxonAnnotate_Map.begin(); it != taxonAnnotate_Map.end(); ++it)
	{
		it->second.RA = ((double)it->second.count/(double)TotalCounts)*100;
		it->second.superkingdom = "";
		it->second.kingdom = "";
		it->second.phylum = "";
		it->second.clas = "";
		it->second.order = "";
		it->second.family = "";
		it->second.genus = "";
		it->second.species = "";
		
		
		taxon_temp = it->first;
		
		while(taxon_temp != 0)
		{
			 rank_temp = rankMap[taxon_temp];
			 
			 
			 if(rank_temp == "species")
			 {
				 it->second.species = namesMap[taxon_temp];
			 }
			 else if (rank_temp == "genus")
			 {
				 it->second.genus = namesMap[taxon_temp];
			 }
			 else if (rank_temp == "family")
			 {
				 it->second.family = namesMap[taxon_temp];
			 }
			 else if (rank_temp == "order")
			 {
				 it->second.order = namesMap[taxon_temp];
			 }
			 else if (rank_temp == "class")
			 {
				 it->second.clas = namesMap[taxon_temp];
			 }
			 else if (rank_temp == "phylum")
			 {
				 it->second.phylum = namesMap[taxon_temp];
			 }
			 else if (rank_temp == "kingdom")
			 {
				 it->second.kingdom = namesMap[taxon_temp];
			 }
			 else if (rank_temp == "superkingdom")
			 {
				 it->second.superkingdom = namesMap[taxon_temp];
			 }
			 else if (rank_temp == "root")
			 {
				 it->second.root = namesMap[taxon_temp];
			 }
			 
			 taxon_temp = standardMap[taxon_temp];
			 
		}
		
		
		//~ cout << it->first << " =" << it->second.count << "\n";
		//~ cout << it->first << " =" << it->second.root << "\n";
		//~ cout << it->first << " =" << it->second.superkingdom << "\n";
		//~ cout << it->first << " =" << it->second.kingdom << "\n";
		//~ cout << it->first << " =" << it->second.phylum << "\n";
		//~ cout << it->first << " =" << it->second.clas << "\n";
		//~ cout << it->first << " =" << it->second.order << "\n";
		//~ cout << it->first << " =" << it->second.family << "\n";
		//~ cout << it->first << " =" << it->second.genus << "\n";
		//~ cout << it->first << " =" << it->second.species << "\n";
	}
	
	/// Export taxonomy and relative abundance to txt file	
	ofstream myfile;
	myfile.open (OutputProfile.c_str());
	if (!myfile.is_open())
	{
		cout << "Error opening Output file: " << OutputProfile << "\n";
		exit(1);	
	}
	
	
	myfile << "taxon" << "," << "count" << "," << "RA" << "," << "root" << "," << "superkingdom" << "," << "kingdom" << "," << "phylum" << "," << "class" << "," << "order" << "," << "family" << ","<< "genus" << "," << "species" << "\n";
	for(map<uint32_t, taxA>::iterator it = taxonAnnotate_Map.begin(); it != taxonAnnotate_Map.end(); ++it)
	{
		myfile << it->second.taxon << "," << it->second.count << "," << it->second.RA << "," << it->second.root << "," << it->second.superkingdom << "," << it->second.kingdom << "," << it->second.phylum << "," << it->second.clas << "," << it->second.order << "," << it->second.family << ","<< it->second.genus << "," << it->second.species << "\n";
	}

	myfile.close();


}
