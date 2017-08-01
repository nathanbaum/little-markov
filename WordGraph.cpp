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

int WordGraph::importGraph(string g){                                           //all the ways g can be deformed:
                                                                                //unequal lengths between semicolons
                                                                                //number of collumns and rows not equal
                                                                                //invalid characters
  if(g[0]==';' || g[0]==':' || g[g.size()-1]==';' || g[g.size()-1]==':') return 1;
  for(int i=0; i<g.size(); i++){
    if(g[i]<'0' || g[i]>';') return 2;                                          //invalid tokens
  }
                                                                                //now that we have completed the input data validation, we can move on to the actual algorithm
  graph.clear();                                                                //clear the old graph to make room for the new one
  int gin = 0;                                                                  //the current index of our manual traversal
  int glen = 1;                                                                 //the number of digits in the urrent number in our traversal
  vector<int> temp;                                                             //temporary vector to build our collumns/rows
  while(gin<g.size()){
    while(gin+glen<g.size() && g[gin+glen]<':'){
      glen++;                                                                   //as long as the next token isn't a delim, keep adding to the length
    }
    temp.push_back(atoi((g.substr(gin, glen)).c_str()));                        //we are turning a string into a substring into a c-string into an int
    gin += glen+1;                                                              //add the length to the current index plus one to hop onto the first digit of the next number
    glen = 1;                                                                   //reset the length for the next loop iteration
    graph.push_back(temp);                                                      //add temp to the graph (it is a new collumn)
    temp.clear();                                                               //clear out temp for the new loop traversal
  }
  return 0;                                                                     //finally we return a success (hopefully)
}

string WordGraph::exportGraph(){
  string out = "";                                                              //start with an empty string
  for(int i=0; i<graph.size(); i++){                                            //traverse collumns
    for(int j=0; j<graph[i].size(); j++){                                       //traverse rows
      out += to_string(graph[i][j]);                                            //add number to the output after converting to string
      if(j+1<graph[i].size()){                                                  //if this isn't the last number in the collumn
        out += ':';                                                             //add in a colon as delim
      }
    }
    if(i+1<graph.size()){                                                       //if this isn't the last collumn in the graph
      out += ';';                                                               //add semicolon as delim
    }
  }
  return out;                                                                   //return that whole string
}

int WordGraph::importDictionary(string d){
  if(d[0]==' ' || d[d.size()-1]==' ') return 1;                                 //can't have a delim at the start or end
  dictionary.clear();                                                           //clear out the dictionary so that we don't append
  int din = 0;                                                                  //same concept as in import graph...
  int dlen = 1;
  while(din < d.size()){
    while(din+dlen<d.size() && d[din+dlen]!=' ' /* && d[din+dlen]!='.' */){
      dlen++;                                                                   //add to length as long as it's not a delim (making sure not to go out of bounds)
    }
    add(d.substr(din,dlen));                                                    //add word to dictionary
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

string WordGraph::exportDictionary(){
  string out = "";                                                              //start with an empty string
  for(int i=0; i<dictionary.size(); i++){                                       //traverse the dictionary
    out += dictionary[i];                                                       //add current word to output string
    if(i+1<dictionary.size()) out+=' ';                                         //for all except last word, add space as delim after
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
