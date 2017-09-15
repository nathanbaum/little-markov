#include "WordGraph.hpp"
#include <getopt.h>

vector<string> fileWords;

string generateChain(WordGraph *wg, int n, string start, string end){           //wg is an initialized WordGraph -- n is the desired length of the chain, if n==-1 it will go until end state
  string out = "";                                                              //if n!=0 end will be ignored
  if(wg->setCurrent(start) == -1){
    return "ERROR: Chain could not be generated because start word is not found in the training data";
  }
  if(n == -1){
    srand(time(NULL));                                                          //generate the seed for the random generator
    while(wg->getCurrent().compare(end) != 0){
      wg->updateCurrent();
      out += wg->getCurrent() + " ";
    }
  }
  else{
    srand(time(NULL));                                                          //generate the seed for the random generator
    for(int i=0; i<n; i++){
      if(wg->updateCurrent()==1) return out;
      out += wg->getCurrent() + " ";
    }
  }
  return out;
}

int addAll(WordGraph *wg, string d){
  int din = 0;                                                                  //same concept as in import graph...
  int dlen = 1;
  while(din < d.size()){
    while(din+dlen<d.size() && d[din+dlen]!=' ' /* && d[din+dlen]!='.' */){
      dlen++;                                                                   //add to length as long as it's not a delim (making sure not to go out of bounds)
    }
    wg->add(d.substr(din,dlen));                                                //add word to dictionary
    fileWords.push_back(d.substr(din,dlen));
    if(d[din+dlen]==' '){
      din += dlen+1;
    }
    else{
      din += dlen;
    }
    dlen = 1;
  }
  return 0;
}

int linkAll(WordGraph *wg){
  cout << "Linking words..." << endl;
  //cout << "Links to make: " << fileWords.size() << endl;
  for(int i=1; i<fileWords.size(); i++){
    //cout << "Linking \"" << fileWords[i-1] << "\" to \"" << fileWords[i] << endl;
    wg->link(fileWords[i-1], fileWords[i]);
    cout << "\r[";
    for(int j=0; j<int (double (i)/double (fileWords.size()-1) * 100); j++){
      cout << "|";
    }
    for(int j=0; j< 100 - int (double (i)/double (fileWords.size()-1) * 100); j++){
      cout << " ";
    }
    cout << "]" << int (double (i)/double (fileWords.size()-1) * 100)<< "%";
  }
  cout << endl;
  fileWords.clear();
  return 0;
}

void printUsage(){
  cout << "Markov";
  cout << "\n";
  cout << "\nUsage:";
  cout <<   "\n\tMarkov -n <number_of_words_in_chain> -s <string_to_start_at> -t <path_to_training_file>... [-e <path_to_export_file> -h]";
  cout << "\n";
  cout << "\nOptions:";
  cout << "\n\t-n\tSpecifies the number of links in the generated Markov chain.";
  cout << "\n\t-s\tSpecifies the starting string for the chain.";
  //cout << "\n\t-i\tThe path to the import file for the Markov chain.";
  cout << "\n\t-t\tThe path to the training file for the Markov chain.";
  cout << "\n\t\tNote: At least one training file must be specified. If multiple are given, the chain will be trained from the files in the order they are given.";
  cout << "\n\t-e\tThe path to the export file for this Markov chain.";
  cout << "\n\t-h\tDisplay this usage document." << endl;
}

void mergeGraphs(WordGraph *g1, WordGraph *g2){
  for(int i=0; i<g1->getDictionary().size(); i++){                              //add all the words from graph 1 to the dictionary of graph 2
    g2->add(g1->getDictionary()[i]);
  }
  for(int i=0; i<g2->getDictionary().size(); i++){                              //iterate through all the words in g2
    for(int j=0; j<g1->getLinks(g2->getDictionary()[i]).size(); j++){           //iterate through all the links in g1 for the word indexed by g2->getDictionary[i]
      g2->link(g2->getDictionary()[i], g1->getDictionary()[g1->getLinks(g2->getDictionary()[i])[j]]);//link the dictionary word from g2 to the word from g1's dictionary indexed by the link at j of that word's collumn in g1
    }//^^^^^^^^^^^THIS IS THE PROBLEMATIC LINE, IT BREAKS IN THE CASE WHERE g2 HAS WORDS THAT g2 DOES NOT
  }
}

int main(int argc, char *argv[]){

  if(argc<2){
    printUsage();
    return 1;
  }

  //WordGraph iGraph;
  vector <WordGraph> tGraphs;
  WordGraph nGraph;

  string STARTING_STRING = "";
  string ENDING_STRING = "";
  int NUM_WORDS = 0;
  //vector<string> IMPORT_FILES;
  bool EXPORT_FLAG = false;
  string EXPORT_NAME = "";
  vector<string> TRAIN_FILES;
  int c;
  bool errFlag = false;

  while((c = getopt (argc, argv, "e:t:s:n:h")) != -1){
    switch (c){
      case 's':
        STARTING_STRING = optarg;
        break;
      case 'n':
        NUM_WORDS = stoi(optarg, nullptr, 10);
        break;
      /*case 'i':
        IMPORT_FILES.push_back(optarg);
        break;*/
      case 'e':
        EXPORT_FLAG = true;
        EXPORT_NAME = optarg;
        break;
      case 't':
        TRAIN_FILES.push_back(optarg);
        break;
      case 'h':
        printUsage();
        return 1;
    }
  }

  if(!(/*IMPORT_FILES.size()>0||*/TRAIN_FILES.size()>0)){
    cout << "ERROR: improper usage" << endl;
    printUsage();
    return 1;
  }

  /*if(IMPORT_FILES.size()>0){
    for(int i=0; i<IMPORT_FILES.size(); i++){
      if(iGraph.import(IMPORT_FILES[i]) != 0){
        cout << "ERROR: file, \"" << IMPORT_FILES[i] << "\" could not be imported" << endl;
      }
    }
  }*/

  for(int i=0; i<TRAIN_FILES.size(); i++){
    ifstream trainFile;
    trainFile.exceptions(ifstream::failbit);
    try{
      trainFile.open(TRAIN_FILES[i], ifstream::in);
      cout << TRAIN_FILES[i] << " opened successfully: reading..." << endl;
    }
    catch(const exception& e){
      cout << "ERROR: " << TRAIN_FILES[i] << " could not be opened" << endl;
      errFlag = true;
    }
    if(!errFlag){
      WordGraph temp;
      tGraphs.push_back(temp);
      string line;
      int lineTotal = 0;
      cout << "counting lines..." << endl;
      while(true){
        try{
          getline(trainFile, line);
        }
        catch(const exception& e){
          break;
        }
        lineTotal++;
      }
      trainFile.close();
      trainFile.open(TRAIN_FILES[i], ifstream::in);
      cout << "Populating dictionary..." << endl;
      int curLine = 0;
      while(true){
        try{
          getline(trainFile, line);
        }
        catch(const exception& e){
          break;
        }
        curLine++;
        cout << "\r[";
        for(int i=0; i<int (double (curLine)/double (lineTotal) * 100); i++){
          cout << "|";
        }
        for(int i=0; i< 100 - int (double (curLine)/double (lineTotal) * 100); i++){
          cout << " ";
        }
        cout << "]" << int (double (curLine)/double (lineTotal) * 100)<< "%";
        addAll(&tGraphs.back(), line);
      }
      trainFile.close();
      cout << endl;
      linkAll(&tGraphs.back());
    }
    errFlag = false;
  }

  if(tGraphs.size()==0){
    cout << "ERROR: training data could not be opened\nexiting..." << endl;
    return 1;
  }
  while(tGraphs.size()>0){
    mergeGraphs(&tGraphs.back(), &nGraph);
    tGraphs.pop_back();
  }

  /*if(!IMPORT_FILES.size()>0 && TRAIN_FILES.size()>0){
    nGraph = tGraph;
  }
  else if (IMPORT_FILES.size()>0 && !TRAIN_FILES.size()>0){
    nGraph = iGraph;
  }
  else{
    cout << "Merging graphs..." << endl;
    mergeGraphs(&iGraph, &tGraph, &nGraph);
  }*/

  cout << "\nThis is what I think you sound like:\n" << generateChain(&nGraph, NUM_WORDS, STARTING_STRING, "") << endl;

  if(EXPORT_FLAG){
    ofstream exportFile;

    cout << "\nExporting graph to " << EXPORT_NAME << endl;
    try{
      exportFile.open (EXPORT_NAME);
    }
    catch(const exception& e){
      cout << "ERROR: export failed\nexiting..." << endl;
      return 1;
    }
    exportFile << nGraph.toString() << endl;
    cout << "Export complete\nSaving..." << endl;
    exportFile.close();
  }
  //cout << "Exported Dictionary:\n" << myGraph.exportDictionary() << endl;
  //cout << "Exported Graph:\n" << myGraph.exportGraph() << endl;


  return 0;
}
