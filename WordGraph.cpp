#include "WordGraph.hpp"

int WordGraph::add(string word){
  //cout << "\nDictionary: " << exportDictionary() << std::endl;
  //cout << "searching for \"" << word << "\"" << endl;
  int index = search(word);
  //cout << "search result: " << index << endl;
  if(index==-1){                                                                //if the dictionary is empty, we don't want to make any weird refs
    //cout << "dictionary empty" << endl;
    dictionary.push_back(word);
    vector <int> w;
    w.push_back(0);
    graph.push_back(w);
    return 0;
  }
  if(index!=dictionary.size() && dictionary[index].compare(word) == 0){         //if a search for the word comes up with a match
    //cout << "word already exists" << endl;
    return 1;                                                                   //return 1, indicating failure
  }                                                                             //if the search found nothing
  //cout << "word does not yet exist - adding" << endl;
  dictionary.insert(dictionary.begin()+index, word);                            //add the word to the dictionary
  vector <int> w;                                                               //make a new temporary vector for that word's collumn
  graph.insert(graph.begin()+index, w);                                         //insert that collumn in the correct index of the graph
  return 0;                                                                     //return 0 indicating success
}

int WordGraph::search(string word){                                             //returns the index for word in dictionary, or the place where word belongs in dictionary
  return search(word, 0, dictionary.size()-1);                                  //it is up to you to check which it is
}

int WordGraph::search(string word, int lo, int hi){
  int mid = ((hi-lo)/2)+lo;
  //cout << "Search: lo=" << lo << " hi=" << hi << " mid=" << mid << endl;
  if(hi<lo){
    return -1;
  }
  int comp = word.compare(dictionary[mid]);
  if(comp==0){
    return mid;
  }
  if(hi == lo+1 && comp<0){
    return lo;
  }
  if(lo==hi){
    if(comp<0) return hi;
    if(comp>0) return hi+1;
  }
  if(comp<0){
    return search(word, lo, mid-1);
  }
  if(comp>0){
    return search(word, mid+1, hi);
  }
  return -1;                                                                    //there's, like, no way for it to get here, but whatever
}

int WordGraph::link(string w1, string w2){
  int iw1=search(w1);                                                           //iw1 stands for index of word 1
  int iw2=search(w2);                                                           //I think you can guess what iw2 stands for
  //cout << "iw1=" << iw1 << endl;
  //cout << "iw2=" << iw2 << endl;
  if(dictionary[iw1].compare(w1)!=0 || dictionary[iw2].compare(w2)!=0) return 1;//if iw doesn't match index, w wasn't found and link failure
  //cout << "Before linking: " << graph[iw1][iw2];
  graph[iw1].push_back(iw2);                                                    //push the index of one of the links to the back of the collumn
  //cout << " After linking: " << graph[iw1][iw2] << endl;
  return 0;
}

int WordGraph::import(string f){
  ifstream importFile;
  importFile.exceptions(ifstream::failbit);
  try{
    importFile.open(f, ifstream::in);
    cout << f << " opened successfully: reading..." << endl;
  }
  catch(const exception& e){
    cout << "ERROR: " << f << " could not be opened" << endl;
    return 1;
  }
  string line;
  int lin = 1;
  int llen = 1;
  vector<int> buf;
  dictionary.clear();
  graph.clear();
  while(true){
    try{
      getline(importFile, line);
    }
    catch(const exception& e){
      break;
    }
    if(line.size() == 0){}
    else if(line[0] == 'd'){
      //cout << line.substr(1,line.size()-1) << endl;
      dictionary.push_back(line.substr(1,line.size()-1));                       //add word to dictionary
    }
    else if(line[0] == 'g'){
      while(lin < line.size()){
        while(lin+llen<line.size() && line[lin+llen]!=','){
          llen++;                                                               //add to length as long as it's not a delim (making sure not to go out of bounds)
        }
        //cout << line.substr(lin, llen) << endl;
        buf.push_back(atoi((line.substr(lin, llen)).c_str()));                  //we are turning a string into a substring into a c-string into an int
        lin += llen+1;                                                          //add the length to the current index plus one to hop onto the first digit of the next number
        llen = 1;                                                               //reset the length for the next loop iteration
      }
      graph.push_back(buf);                                                     //add temp to the graph (it is a new collumn)
      buf.clear();                                                              //clear out temp for the new loop traversal
      lin = 1;
    }
  }
  importFile.close();
  return 0;
}

string WordGraph::toString(){
  string out = "";                                                              //start with an empty string
  for(int i=0; i<dictionary.size(); i++){                                       //traverse the dictionary
    out += "d" + dictionary[i];                                                 //add current word to output string
    out+='\n';                                                                  //add new line
  }
  for(int i=0; i<dictionary.size(); i++){                                       //traverse the graph
    out += "g";
    for(int j=0; j<graph[i].size(); j++){
      out += to_string(graph[i][j]);
      if(j+1<graph[i].size()) out+=',';
    }
    if(i+1<dictionary.size()) out+='\n';                                        //for all except last word, add space as delim after
  }
  return out;                                                                   //return our output string
}

string WordGraph::getCurrent(){                                                 //BE CAREFUL WHEN CURRENT IS UNINITIALIZED
  return dictionary[current];
}

int WordGraph::setCurrent(string s){
  int index = search(s);                                                        //search for s
  if(dictionary[index].compare(s)!=0){                                          //if it's not there
    return -1;
  }                                                                             //if it is there
  current = index;                                                              //set current to its index
  return 0;                                                                     //return success
}

int WordGraph::updateCurrent(){                                                 //MAKE SURE TO SEED THE RANDOM NUMBER GEN BEFORE RUNNING!
  if(graph[current].size() == 0) return 1;                                      //if there is nowhere to go, return with an error
  //cout << "random number: " << rand() % chooseNext.size() << endl;
  current = graph[current][rand() % graph[current].size()];                     //set current to the index from our random number landing somewhere in our new vector
  //for(int i=0; i<graph[current].size(); i++){
  //  cout << graph[current][i] << " ";
  //}
  //cout << endl;
  return 0;                                                                     //return success
}

const vector <string> WordGraph::getDictionary(){
  return dictionary;
}

vector <int> WordGraph::getLinks(string w){
  int iw=search(w);                                                             //iw stands for index of word

  if(dictionary[iw].compare(w)!=0){
    vector<int> ret;
    ret.push_back(-1);
    return ret;                                                                  //if iw doesn't match index, w wasn't found
  }

  return graph[iw];
}
