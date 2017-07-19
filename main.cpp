#include <fstream>
#include "WordGraph.hpp"

string generateChain(WordGraph wg, int n, string start, string end){            //wg is an initialized WordGraph -- n is the desired length of the chain, if n==-1 it will go until end state
  string out = "";                                                              //if n!=0 end will be ignored
  wg.setCurrent(start);
  if(n == -1){
    while(wg.getCurrent().compare(end) != 0){
      wg.updateCurrent();
      out += wg.getCurrent() + " ";
    }
  }
  else{
    for(int i=0; i<n; i++){
      wg.updateCurrent();
      out += wg.getCurrent() + " ";
    }
  }
  return out;
}

WordGraph addAll(WordGraph wg, string d){
  int din = 0;                                                                  //same concept as in import graph...
  int dlen = 1;
  while(din < d.size()){
    while(din+dlen<d.size() && d[din+dlen]!=' ' && d[din+dlen]!='.'){
      dlen++;                                                                   //add to length as long as it's not a delim (making sure not to go out of bounds)
    }
    wg.add(d.substr(din,dlen));                                                 //add word to dictionary
    if(d[din+dlen]==' '){
      din += dlen+1;
    }
    else{
      din += dlen;
    }
    dlen = 1;
  }
  return wg;
}

WordGraph linkAll(WordGraph wg, string d){
  vector<string> pair;
  int din = 0;                                                                  //same concept as in import graph...
  int dlen = 1;
  while(din < d.size()){
    while(din+dlen<d.size() && d[din+dlen]!=' ' && d[din+dlen]!='.'){
      dlen++;                                                                   //add to length as long as it's not a delim (making sure not to go out of bounds)
    }
    pair.insert(pair.begin(), d.substr(din,dlen));                              //push the substring to the begining of pair
    if(pair.size() == 2){                                                       //when you get to two
      wg.link(pair[1], pair[0]);                                                //now link the first word to the second word
      pair.pop_back();                                                          //pop off the last (actually first) word
    }
    if(d[din+dlen]==' '){
      din += dlen+1;
    }
    else{
      din += dlen;
    }
    dlen = 1;
  }
  return wg;
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
    int percentComplete = 0;
    int curLine = 0;
    while(true){
      try{
        getline(file, line);
      }
      catch(const exception& e){
        break;
      }
      curLine++;
      if(int (double (curLine)/double (lineTotal) * 100) != percentComplete){
        cout << "\r[";
        for(int i=0; i<int (double (curLine)/double (lineTotal) * 100); i++){
          cout << "|";
        }
        for(int i=0; i< 100 - int (double (curLine)/double (lineTotal) * 100); i++){
          cout << " ";
        }
        cout << "]" << int (double (curLine)/double (lineTotal) * 100)<< "%";
        percentComplete = int (double (curLine)/double (lineTotal) * 100);
      }
      myGraph = addAll(myGraph, line);
    }
    file.close();
    myGraph.buildGraph();
    file.open(argv[1], ifstream::in);
    cout << "Linking words..." << endl;
    percentComplete = 0;
    curLine = 0;
    while(true){
      try{
        getline(file, line);
      }
      catch(const exception& e){
        break;
      }
      curLine++;
      if(int (double (curLine)/double (lineTotal) * 100) != percentComplete){
        cout << "\r[";
        for(int i=0; i<int (double (curLine)/double (lineTotal) * 100); i++){
          cout << "|";
        }
        for(int i=0; i< 100 - int (double (curLine)/double (lineTotal) * 100); i++){
          cout << " ";
        }
        cout << "]" << int (double (curLine)/double (lineTotal) * 100)<< "%";
        percentComplete = int (double (curLine)/double (lineTotal) * 100);
      }
      myGraph = linkAll(myGraph, line);
    }
    file.close();
    cout << endl;
    //cout << "Exported Dictionary:\n" << myGraph.exportDictionary() << endl;
    //cout << "Exported Graph:\n" << myGraph.exportGraph() << endl;

    cout << "This is what I think you sound like:\n" << generateChain(myGraph, 200, ".", "") << endl;

    ofstream dictionaryExport, graphExport;

    dictionaryExport.open ("dictionary.txt");
    dictionaryExport << myGraph.exportDictionary();
    dictionaryExport.close();

    graphExport.open ("graph.txt");
    graphExport << myGraph.exportGraph();
    graphExport.close();
    return 0;
  }
  else{
    cout << "no file argument found: exiting" << endl;
    return 1;
  }
}
