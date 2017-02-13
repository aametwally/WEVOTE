#include "headers.hpp"
#include "helpers.hpp"
#include "create_db.hpp"



map<uint32_t, uint32_t> parentMap;
map<uint32_t, string> rankMap;
map<uint32_t, uint32_t> standardMap;
map<uint32_t, string> namesMap;
uint32_t undefined=0;


int main(int argc, char *argv[])
{
	
	string nodesFilename="";
	string namesFilename="";
	string query="";
	string prefix="";
	string taxonomyDB="";
	string abundanceMethod="total";
	
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
			{"t", 1, 0, 0} 
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
				default: break; 
			}
		}
	}
	
	
	nodesFilename=taxonomyDB+"/nodes_wevote.dmp";
	namesFilename=taxonomyDB+"/names_wevote.dmp";
	string OutputProfile= prefix + "_Abundance.txt";
	vector<taxon> TaxaInfo;
	taxon OneTaxon;
	vector<uint32_t> InputTaxa;
	TaxaInfo.clear();
	InputTaxa.clear();
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

	while (getline(file, line))
	{
		stringstream strstr(line);
		string word = "";
		it=0;

			
		while (getline(strstr,word, ',')) 
		{			
			if(it==1)
			{
				InputTaxa.push_back(atoi(word.c_str()));
				break;
			}
											
			it++;
		}		
	}
	
	
	
	
	uint32_t numSeq= InputTaxa.size();
	uint32_t numClassifiedReads=0;	
	map<uint32_t, uint32_t> hit_counts;
	set<uint32_t> parents;
	std::set<uint32_t>::iterator set_iterator;
	std::map<uint32_t, uint32_t>::iterator iterator_name; 	
	hit_counts.clear();




	/// correct for the nonstandard taxa
	 correctTaxa_vecTaxa(InputTaxa);


	/// Build weighted Tree for all taxa
	for (uint32_t j=0 ; j<numSeq ; j++)
	{
		if(InputTaxa[j]!=0)
		{

		parents=get_ancestry(InputTaxa[j]);
		for(set_iterator = parents.begin(); set_iterator != parents.end(); set_iterator++)
		{
			//cout << "ancestoy= " << *set_iterator	 << "\n";
			hit_counts[*set_iterator]++;
		}
	
			numClassifiedReads++;
		}			
	}
	
	
	cout << "num of seq=" << numSeq << "\n";
	cout << "Number of classified reads= " << numClassifiedReads << "\n";
	cout << "num of classified reads=" << numClassifiedReads << "\n";	
	cout << "standard parent of 1396=" << standardMap[1396] << "\n";
	cout << "normal parent of 1396=" << parentMap[1396] << "\n";
	cout << "standard parent of 86661=" << standardMap[86661] << "\n";
	cout << "normal parent of 86661=" << parentMap[86661] << "\n";
	




	/// Assign rank, name, and abundance based on the total number of reads	
	if(abundanceMethod=="classified")
	{
		cout<<"Abundance calculation method = Classified \n";
		
		for(iterator_name = hit_counts.begin(); iterator_name != hit_counts.end(); iterator_name++)
		{
		OneTaxon.taxonID=iterator_name->first;
		OneTaxon.occurrence=iterator_name->second;
		OneTaxon.rank=rankMap[OneTaxon.taxonID];
		OneTaxon.name=namesMap[OneTaxon.taxonID];
		OneTaxon.abundance=((double)OneTaxon.occurrence/(double)numClassifiedReads)*100;
		
		TaxaInfo.push_back(OneTaxon);
		}
	}


	/// Assign rank, name, and abundance based on the number of classified reads only
	else if(abundanceMethod=="total")	
	{
		cout<<"Abundance calculation method = total \n";
		
		for(iterator_name = hit_counts.begin(); iterator_name != hit_counts.end(); iterator_name++)
		{
		OneTaxon.taxonID=iterator_name->first;
		OneTaxon.occurrence=iterator_name->second;
		OneTaxon.rank=rankMap[OneTaxon.taxonID];
		OneTaxon.name=namesMap[OneTaxon.taxonID];
		OneTaxon.abundance=((double)OneTaxon.occurrence/(double)numSeq)*100;

		TaxaInfo.push_back(OneTaxon);
		}
	}
	else
	{	
		cout<<"Wrong abundance calculation method \n";
		exit(1);
	}	




	
	 for (uint32_t i=0; i<hit_counts.size() ; i++)
	 {
		cout<< i << "\t" <<TaxaInfo[i].taxonID << "\t" << TaxaInfo[i].occurrence << "\t" << TaxaInfo[i].abundance << "\t" << TaxaInfo[i].rank << "\t" << TaxaInfo[i].name << "\n";
	 }					
	
	
	
	/// Export taxonomy and relative abundance to txt file	
	ofstream myfile;
	myfile.open (OutputProfile.c_str());
	if (!myfile.is_open())
	{
		cout << "Error opening Output file: " << OutputProfile << "\n";
		exit(1);	
	}
	for (uint32_t i=0 ; i<hit_counts.size() ;i++)
	{	
		myfile << TaxaInfo[i].taxonID << "\t" << TaxaInfo[i].occurrence << "\t" << TaxaInfo[i].abundance << "\t" << TaxaInfo[i].rank << "\t" << TaxaInfo[i].name << "\n";
	}
	myfile.close();
}
