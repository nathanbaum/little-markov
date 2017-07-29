#include <fstream>
#include "WordGraph.hpp"

vector<string> fileWords;

string generateChain(WordGraph *wg, int n, string start, string end){            //wg is an initialized WordGraph -- n is the desired length of the chain, if n==-1 it will go until end state
  string out = "";                                                              //if n!=0 end will be ignored
  if(wg->setCurrent(start) == -1){
    return "Chain could not be generated because start word is not found in the training data.";
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
    while(din+dlen<d.size() && d[din+dlen]!=' ' /*&& d[din+dlen]!='.'*/){
      dlen++;                                                                   //add to length as long as it's not a delim (making sure not to go out of bounds)
    }
    wg->add(d.substr(din,dlen));                                                 //add word to dictionary
    fileWords.push_back(d.substr(din,dlen));
    //if(fileWords.size()>1){
    //  wg.link(fileWords.back(), fileWords.front());
    //  fileWords.pop_back();
    //}
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

int main(int argc, char *argv[]){

  WordGraph myGraph;

  if(argc>1){
    ifstream file;
    file.exceptions(ifstream::failbit);
    try{
      file.open(argv[1], ifstream::in);
      cout << argv[1] << " opened successfully: reading..." << endl;
    }
    catch(const exception& e){
      cout << argv[1] << " could not be opened: exiting" << endl;
      return 1;
    }
    string line;
    int lineTotal = 0;
    cout << "counting lines..." << endl;
    while(true){
      try{
        getline(file, line);
      }
      catch(const exception& e){
        break;
      }
      lineTotal++;
    }
    file.close();
    file.open(argv[1], ifstream::in);
    cout << "Populating dictionary..." << endl;
    int curLine = 0;
    while(true){
      try{
        getline(file, line);
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
      addAll(&myGraph, line);
    }
    file.close();
    cout << endl;
    myGraph.buildGraphVerbose();
    linkAll(&myGraph);
    //cout << "Exported Dictionary:\n" << myGraph.exportDictionary() << endl;
    //cout << "Exported Graph:\n" << myGraph.exportGraph() << endl;

    cout << "\nThis is what I think you sound like:\n" << generateChain(&myGraph, 200, "the", "") << endl;

    //ofstream dictionaryExport, graphExport;

    //cout << "Exporting dictionary to dictionary.txt..." << endl;
    //dictionaryExport.open ("dictionary.txt");
    //dictionaryExport << myGraph.exportDictionary();
    //dictionaryExport.close();

    //cout << "Exporting graph to graph.txt" << endl;
    //graphExport.open ("graph.txt");
    //graphExport << myGraph.exportGraph();
    //graphExport.close();
    return 0;
  }
  else{
    cout << "no file argument found: exiting" << endl;
    return 1;
  }
}
