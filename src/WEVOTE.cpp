#include "headers.hpp"
#include "helpers.hpp"
#include "create_db.hpp"


void usage(string prog);

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
	bool verbose=false;
	uint32_t score=0;
	int threads=1;
	int penalty=2;
	int minNumAgreed=0;
	
	if (argc<2)
	{
		cout << "less than the required minimum number of options\n";
		usage(argv[0]);
		exit(1);
	}
	
	
	/// parse commandline arguments
	while (1) 
	{	
		static struct option long_options[] = 
		{			 
			{"v", 0, 0, 0},
			{"i", 1, 0, 0},
			{"p", 1, 0, 0},
			{"d", 1, 0, 0},
			{"s", 1, 0, 0},
			{"n", 1, 0, 0},
			{"k", 1, 0, 0},
			{"a", 1, 0, 0} 
		};
		
		int longindex = -1;
		int c = getopt_long_only(argc, argv, "", long_options, &longindex);
		if(c == -1) break;
		else if(c == '?') 
		{
			/// If the user entered junk, let him know. 
			cerr << "Invalid parameters." << endl;
			usage(argv[0]);
			exit(1);
		}
		else 
		{
			switch(longindex) 
			{ 
				case 0: verbose = true; break;
				case 1: query=optarg; break;
				case 2: prefix=optarg; break;
				case 3: taxonomyDB=optarg; break;
				case 4: score=atol(optarg); break;
				case 5: threads=atol(optarg); break;
				case 6: penalty=atol(optarg); break;
				case 7: minNumAgreed=atol(optarg); break;
				default: break; 
			}
		}
	}
	
	
	nodesFilename=taxonomyDB+"/nodes_wevote.dmp"; 
	namesFilename=taxonomyDB+"/names_wevote.dmp";	
	string OutputDetails= prefix + "_WEVOTE_Details.txt";
	string OutputWEVOTE= prefix + "_WEVOTE.csv";
	cout << "NodesFilename= " << nodesFilename << "\n";
	cout << "NamesFilename= " << namesFilename << "\n";
	
	/// Build taxonomy trees
	parentMap = build_full_taxid_map(nodesFilename);	
	rankMap = build_full_rank_map(nodesFilename);
	standardMap=build_standard_taxid_map(nodesFilename, parentMap, rankMap);
	namesMap = build_taxname_map(namesFilename);
	
	
	/// Read CSV formated input file  
	vector<readsInfo> Reads;
	readsInfo OneRead;
	Reads.clear();
	ifstream file (query.c_str());    
	string line = "";
	int it=0;
	
	
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
		int value=0;
		(OneRead.annotation).clear();
		
		while (getline(strstr,word, ',')) 
		{			
			if(it==0)
				OneRead.seqID=word;
			else
			{
				value=atoi(word.c_str());
				(OneRead.annotation).push_back(value);
			}								
			it++;
		}		
		Reads.push_back(OneRead);
	}
	   
    
	uint32_t numToolsUsed=(Reads[0].annotation).size();
	uint32_t numReads= Reads.size();
	
	cout << "Num of reads= " << numReads << "\n";
	cout << "Num of used tools= " << numToolsUsed << "\n";
	
	if(minNumAgreed>numToolsUsed)
	{
		cout<< "It's not allwed that minNumAgreed > numTools \n";
		exit(1);
	}
	
	for (uint32_t i=0 ; i<Reads.size() ; i++)
	{
		Reads[i].numToolsUsed=numToolsUsed;
	}
     
    
    
    /// Correct TaxonID for the not standard ranks   
    correctTaxa(Reads);
   	
    /// Count number of tools that identified each sequences
    uint32_t count=0;
	for (uint32_t i=0 ; i<Reads.size() ; i++)
	{	
		count=0;
		for (uint32_t j=0 ; j<(Reads[i].annotation).size() ; j++)
		{       
			
			if (Reads[i].annotation[j] && 1)
			count++;			
		}
		Reads[i].numToolsReported=count;
	}	
	
	
	
	/// WEVOTE algorithm
	map<uint32_t, uint32_t> hit_counts;
	set<uint32_t> parents;
	std::set<uint32_t>::iterator set_iterator;
    std::map<uint32_t, uint32_t>::iterator iterator_name; 
    uint32_t SavedTax_2[2]; 
    uint32_t n=0; 
     
    double start = omp_get_wtime();    
    omp_set_num_threads(threads);
	cout << "Min Number of Agreed= "<< minNumAgreed << "\n"; 
    
    #pragma omp parallel for private (hit_counts, parents, set_iterator, iterator_name, SavedTax_2, n)
	for (uint32_t i=0 ; i<Reads.size() ;i++)
	{	
		if (Reads[i].numToolsReported == 0)
		{
			Reads[i].resolvedTaxon=0;
			Reads[i].numToolsAgreed=numToolsUsed;
			Reads[i].score=1;
			continue;
		}
		else if(Reads[i].numToolsReported == 1 && minNumAgreed<=1)
		{
			Reads[i].resolvedTaxon=0;
			for (uint32_t j=0 ; j<numToolsUsed ; j++)
			{
				Reads[i].resolvedTaxon += Reads[i].annotation[j];
			} 			
			Reads[i].numToolsAgreed=1;
			if(Reads[i].numToolsAgreed==Reads[i].numToolsReported)
			{
				Reads[i].score=(double)Reads[i].numToolsAgreed / (double)Reads[i].numToolsUsed;
			}
			else
			{
				Reads[i].score=((double)Reads[i].numToolsAgreed / (double)Reads[i].numToolsUsed) - (1/penalty*((double)Reads[i].numToolsUsed));
			}
			continue;
		}
		else if(Reads[i].numToolsReported==2 && minNumAgreed<=2)
		{
			n=0;
			
			for (uint32_t j=0 ; j<numToolsUsed ; j++)
			{
				if(Reads[i].annotation[j])
				{
					SavedTax_2[n]=Reads[i].annotation[j];
					n++;
				}
			}
			
			Reads[i].resolvedTaxon=lca(SavedTax_2[0], SavedTax_2[1]);
			Reads[i].numToolsAgreed=2;
			if(Reads[i].numToolsAgreed==Reads[i].numToolsReported)
			{
				Reads[i].score=(double)Reads[i].numToolsAgreed / (double)Reads[i].numToolsUsed;
			}
			else
			{
				Reads[i].score=(double)Reads[i].numToolsReported / (double)(Reads[i].numToolsUsed * Reads[i].numToolsAgreed);
			}
			continue;
		}
		else if(Reads[i].numToolsReported>=3)
		{		
			hit_counts.clear();
			for (uint32_t j=0 ; j<numToolsUsed ; j++)
			{
				parents=get_ancestry(Reads[i].annotation[j]);
				for(set_iterator = parents.begin(); set_iterator != parents.end(); set_iterator++)
				{	
					hit_counts[*set_iterator]++;
				}			
			}				
								
			/// Resolve tree for each sequence to get WEVOTE annotation  
			Reads[i].resolvedTaxon=resolve_tree(hit_counts, Reads[i].numToolsReported, minNumAgreed);
			Reads[i].numToolsAgreed=hit_counts[Reads[i].resolvedTaxon];
			
			if(Reads[i].numToolsAgreed==Reads[i].numToolsReported)
			{
				Reads[i].score=(double)Reads[i].numToolsAgreed / (double)Reads[i].numToolsUsed;
			}
			else
			{
				Reads[i].score=((double)Reads[i].numToolsAgreed / (double)(Reads[i].numToolsUsed)) - (1/ (penalty * ((double)Reads[i].numToolsUsed)));
			}
		}
		else
		{
			Reads[i].resolvedTaxon=0;
                        Reads[i].numToolsAgreed=0;
                        Reads[i].score=0;
                        continue;
		}		 				
	}
	double end = omp_get_wtime();
	double total=end-start;
	cout << "WEVOTE classification executed in= " << total << "\n";


	/// Export the detailed output in txt format 
	ofstream myfile;
	myfile.open (OutputDetails.c_str());
	if (!myfile.is_open())
	{
		cout << "Error opening Output file: " << OutputDetails << "\n";
		exit(1);		
	}
		
	string PrintString="";
	std::ostringstream ss;
	for (uint32_t i=0 ; i<Reads.size() ;i++)
	{	
		ss.clear();
		ss.str("");	
		PrintString="";
		for (uint32_t j=0 ; j<(Reads[i].annotation).size() ; j++)
		{ 	
			ss.clear();
			ss.str("");
			PrintString = PrintString + "\t";
			ss << Reads[i].annotation[j];
			PrintString = PrintString + ss.str();
		}
		
		myfile << Reads[i].seqID << "\t" << Reads[i].numToolsAgreed<<"\t"<< Reads[i].numToolsReported << "\t" << Reads[i].score << "\t" << Reads[i].resolvedTaxon << PrintString << "\n";
		
	}	
	myfile.close();
	
	
	
	/// Export WEVOTE output in CSV format <seqID, taxID, score, # toolsAgreed>	
	myfile.open (OutputWEVOTE.c_str());
	if (!myfile.is_open())
	{
		cout << "Error opening Output file: " << OutputWEVOTE << "\n";
		exit(1);	
	}
	for (uint32_t i=0 ; i<Reads.size() ;i++)
	{	
		myfile << Reads[i].seqID << "," << Reads[i].resolvedTaxon << "," << Reads[i].numToolsAgreed << "," << Reads[i].score <<"\n";
	}
	myfile.close();
	
				
}


void usage(string prog) 
{
	cerr << "Usage: " << prog << " -i <input-file> [options]" << endl;
	cerr << "\n";
	cerr << "Implemented Options:" << endl;
	cerr << "-i      InputFile" << endl;
	cerr << "-d      Taxonomy database path" << endl;
	cerr << "-p      OutputFile Prefix" << endl;
	cerr << "-n      Num of threads" << endl;
	cerr << "-k      Penalty " << endl;
	cerr << "-a      Minimum number of tools agreed tools on WEVOTE decision " << endl;
	cerr << "-s      Score threshold " << endl;

	exit(1);
}
