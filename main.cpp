#include <fstream>
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
  cout <<   "\n\tMarkov -n <number_of_words_in_chain> -s <string_to_start_at> (-i <path_to_import_file> &| -t <path_to_training_file>) [-e <path_to_export_file> -h]";
  cout << "\n";
  cout << "\nOptions:";
  cout << "\n\t-n\tSpecifies the number of links in the generated Markov chain.";
  cout << "\n\t-s\tSpecifies the starting string for the chain.";
  cout << "\n\t-i\tThe path to the import file for the Markov chain.";
  cout << "\n\t-t\tThe path to the training file for the Markov chain.";
  cout << "\n\t\tNote: -i, -t, or both must be specified. If both options are given, the chain will be imported from the file, then trained additionally on the training file.";
  cout << "\n\t-e\tThe path to the export file for this Markov chain.";
  cout << "\n\t-h\tDisplay this usage document." << endl;
}

void mergeGraphs(WordGraph *g1, WordGraph *g2, WordGraph *og){
  //now we need to combine the imported graph with the trained graph
  for(int i=0; i<g1->getDictionary().size(); i++){                              //add all the words from both graphs to the dictionary
    og->add(g1->getDictionary()[i]);
  }
  for(int i=0; i<g2->getDictionary().size(); i++){
    og->add(g2->getDictionary()[i]);
  }
  for(int i=0; i<og->getDictionary().size(); i++){
    for(int j=0; j<g1->getLinks(og->getDictionary()[i]).size(); j++){
      og->link(og->getDictionary()[i], g1->getDictionary()[g1->getLinks(og->getDictionary()[i])[j]]);//link the dictionary word from og to the word from g1's dictionary indexed by the link at j of that word's collumn in g1
    }
    for(int j=0; j<g2->getLinks(og->getDictionary()[i]).size(); j++){
      og->link(og->getDictionary()[i], g2->getDictionary()[g2->getLinks(og->getDictionary()[i])[j]]);
    }
  }
}

int main(int argc, char *argv[]){

  if(argc<2){
    printUsage();
    return 1;
  }

  WordGraph iGraph;
  WordGraph tGraph;
  WordGraph nGraph;

  string STARTING_STRING = "";
  string ENDING_STRING = "";
  int NUM_WORDS = 0;
  bool IMPORT_FLAG = false;
  string IMPORT_NAME = "";
  bool EXPORT_FLAG = false;
  string EXPORT_NAME = "";
  bool TRAIN_FLAG = false;
  string TRAIN_FILE = "";
  int c;
  bool errFlag = false;

  while((c = getopt (argc, argv, "i:e:t:s:n:h")) != -1){
    switch (c){
      case 's':
        STARTING_STRING = optarg;
        break;
      case 'n':
        NUM_WORDS = stoi(optarg, nullptr, 10);
        break;
      case 'i':
        IMPORT_FLAG = true;
        IMPORT_NAME = optarg;
        break;
      case 'e':
        EXPORT_FLAG = true;
        EXPORT_NAME = optarg;
        break;
      case 't':
        TRAIN_FLAG = true;
        TRAIN_FILE = optarg;
        break;
      case 'h':
        printUsage();
        break;
    }
  }

  if(!(IMPORT_FLAG||TRAIN_FLAG)){
    cout << "ERROR: improper usage" << endl;
    printUsage();
    return 1;
  }

  if(IMPORT_FLAG){
    ifstream importFile;
    importFile.exceptions(ifstream::failbit);
    try{
      importFile.open(IMPORT_NAME, ifstream::in);
      cout << IMPORT_NAME << " opened successfully: reading..." << endl;
    }
    catch(const exception& e){
      cout << "ERROR: " << IMPORT_NAME << " could not be opened" << endl;
      if(!TRAIN_FLAG){
        cout << "FATAL ERROR: no training or import data\nexiting..." << endl;
        return 1;
      }
      errFlag = true;
    }
    if(!errFlag){
      string line;
      try{
        getline(importFile, line);
      }
      catch(const exception& e){
        cout << "ERROR: improperly formatted import file" << endl;
        errFlag = true;
      }
      if(!errFlag){
        if(iGraph.importDictionary(line)!=0){
          cout << "ERROR: improperly formatted dictionary" << endl;
          errFlag = true;
        }
        if(!errFlag){
          try{
            getline(importFile, line);
          }
          catch(const exception& e){
            cout << "ERROR: improperly formatted import file" << endl;
            errFlag = true;
          }
          if(!errFlag){
            if(iGraph.importGraph(line)!=0){
              cout << "ERROR: improperly formatted graph" << endl;
            }
          }
        }
      }
      importFile.close();
    }
    if(errFlag && !TRAIN_FLAG){
      cout << "FATAL ERROR: no training or import data\nexiting..." << endl;
      return 1;
    }
  }

  if(TRAIN_FLAG){
    ifstream trainFile;
    trainFile.exceptions(ifstream::failbit);
    try{
      trainFile.open(TRAIN_FILE, ifstream::in);
      cout << TRAIN_FILE << " opened successfully: reading..." << endl;
    }
    catch(const exception& e){
      cout << "ERROR: " << TRAIN_FILE << " could not be opened" << endl;
      if(errFlag){
        cout << "FATAL ERROR: no training or import data\nexiting..." << endl;
        return 1;
      }
      errFlag = true;
    }
    if(!errFlag){
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
      trainFile.open(TRAIN_FILE, ifstream::in);
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
        if(IMPORT_FLAG)addAll(&tGraph, line);
        else addAll(&tGraph, line);
      }
      trainFile.close();
      cout << endl;
      linkAll(&tGraph);
    }
  }

  if(!IMPORT_FLAG && TRAIN_FLAG){
    nGraph = tGraph;
  }
  else if (IMPORT_FLAG && !TRAIN_FLAG){
    nGraph = iGraph;
  }
  else{
    cout << "Merging graphs..." << endl;
    mergeGraphs(&iGraph, &tGraph, &nGraph);
  }

  cout << "\nThis is what I think you sound like:\n" << generateChain(&nGraph, NUM_WORDS, STARTING_STRING, "") << endl;

  if(EXPORT_FLAG){
    ofstream exportFile;

    cout << "\nExporting graph to " << EXPORT_NAME << " -- this might take a while..." << endl;
    try{
      exportFile.open (EXPORT_NAME);
    }
    catch(const exception& e){
      cout << "ERROR: export failed\nexiting..." << endl;
      return 1;
    }
    exportFile << nGraph.exportDictionary() << endl;
    cout << "Dictionary export complete" << endl;
    exportFile << nGraph.exportGraph();
    cout << "Graph export complete\nSaving..." << endl;
    exportFile.close();
  }
  //cout << "Exported Dictionary:\n" << myGraph.exportDictionary() << endl;
  //cout << "Exported Graph:\n" << myGraph.exportGraph() << endl;


  return 0;
}
