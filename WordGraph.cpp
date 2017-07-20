#include "WordGraph.hpp"

int WordGraph::add(string word){
  //cout << "\nDictionary: " << exportDictionary() << std::endl;
  //cout << "searching for \"" << word << "\"" << endl;
  int index = search(word);
  //cout << "search result: " << index << endl;
  if(index==-1){                                                                //if the dictionary is empty, we don't want to make any weird refs
    //cout << "dictionary empty" << endl;
    dictionary.push_back(word);
    return 0;
  }
  if(index!=dictionary.size() && dictionary[index].compare(word) == 0){                                     //if a search for the word comes up with a match
    //cout << "word already exists" << endl;
    return 1;                                                                   //return 1, indicating failure
  }                                                                             //if the search found nothing
  auto it = dictionary.begin();
  //cout << "word does not yet exist - adding" << endl;
  dictionary.insert(it+index, word);                                            //add the word to the dictionary
  return 0;                                                                     //return 0 indicating success
}

int WordGraph::add(string word, int index){
  if(dictionary[index].compare(word) == 0){                                     //if index and word match
    return 1;                                                                   //return 1, indicating failure
  }                                                                             //if the search found nothing
  auto it = dictionary.begin();
  dictionary.insert(it+index, word);                                            //add the word to the dictionary
  return 0;                                                                     //return 0 indicating success
}

int WordGraph::search(string word){                                             //returns the index for word in dictionary, or the place where word belongs in dictionary
  //if(dictionary.size()==0){
  //  return -1;
  //}
  //cout << "dictionary size: " << dictionary.size() << endl;
  return search(word, 0, dictionary.size()-1);                                  //it is up to you to check which it is
}

int WordGraph::search(string word, int lo, int hi){                             //FIGURE THIS SHIT OUT!!! BECAUSE IT DOESN'T WORK, OH AT ALL
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

int WordGraph::buildGraph(){
  vector <int> w;                                                               //make a new temporary vector for that word's collumn
  for(int i=0; i<dictionary.size(); i++){                                       //and fill it with zeros
    w.push_back(0);
  }
  for(int i=0; i<dictionary.size(); i++){
    graph.push_back(w);                                                         //now add that collumn to the graph
  }
}

int WordGraph::link(string w1, string w2){
  int iw1=search(w1);                                                           //iw1 stands for index of word 1
  int iw2=search(w2);                                                           //I think you can guess what iw2 stands for
  //cout << "iw1=" << iw1 << endl;
  //cout << "iw2=" << iw2 << endl;
  if(dictionary[iw1].compare(w1)!=0 || dictionary[iw2].compare(w2)!=0) return 1;//if iw doesn't match index, w wasn't found and link failure
  //cout << "Before linking: " << graph[iw1][iw2];
  graph[iw1][iw2]++;                                                            //increment that spot on graph by one to link w1 to w2
  //cout << " After linking: " << graph[iw1][iw2] << endl;
  return 0;
}

int WordGraph::importGraph(string g){                                           //all the ways g can be deformed:
                                                                                //unequal lengths between semicolons
                                                                                //number of collumns and rows not equal
                                                                                //invalid characters
  int graphSize = -1;
  int rowCounter = 0;
  int collumnCounter = 0;
  if(g[0]==';' || g[0]==':' || g[g.size()-1]==';' || g[g.size()-1]==':') return 1;
  for(int i=0; i<g.size(); i++){
    if(g[i]<'0' || g[i]>';') return 2;                                          //invalid tokens
    else if(g[i]==';'){
      if(graphSize==-1) graphSize = rowCounter+1;                               //setting graph size based on the number of rows in first collumn
      if(rowCounter<graphSize-1) return 1;                                      //at least one row is too small
      rowCounter = 0;
      collumnCounter++;
    }
    else if(g[i]==':'){
      rowCounter++;
      if(graphSize!=-1 && rowCounter>graphSize-1) return 1;                     //at least one row is too big
    }
  }
  if(graphSize == -1) return 1;                                                 //malformed input
  if(collumnCounter != graphSize-1) return 1;                                   //there should be an equal number of collumns and rows
                                                                                //now that we have completed the input data validation, we can move on to the actual algorithm
  graph.clear();                                                                //clear the old graph to make room for the new one
  int gin = 0;                                                                  //the current index of our manual traversal
  int glen = 1;                                                                 //the number of digits in the urrent number in our traversal
  vector<int> temp;                                                             //temporary vector to build our collumns/rows
  for(int i=0; i<graphSize; i++){                                               //collumn traversal
    for(int j=0; j<graphSize; j++){                                             //row traversal
      while(gin+glen<g.size() && g[gin+glen]<':'){
        glen++;                                                                 //as long as the next token isn't a delim, keep adding to the length
      }
      temp.push_back(atoi((g.substr(gin, glen)).c_str()));                      //we are turning a string into a substring into a c-string into an int
      gin += glen+1;                                                            //add the length to the current index plus one to hop onto the first digit of the next number
      glen = 1;                                                                 //reset the length for the next loop iteration
    }
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
    while(din+dlen<d.size() && d[din+dlen]!=' '){
      dlen++;                                                                   //add to length as long as it's not a delim (making sure not to go out of bounds)
    }
    dictionary.push_back(d.substr(din,dlen));                                   //push the substring to the end of dictionary
    din += dlen+1;
    dlen = 1;
  }
  return 0;                                                                     //return success
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
  vector<int> chooseNext;
  for(int i=0; i<graph[current].size(); i++){                                   //traverse the column in the graph
    for(int j=0; j<graph[current][i]; j++){                                     //for the number of links at graph[current][i]
      chooseNext.push_back(i);                                                  //push the index of that to the end of the vector
    }                                                                           //now we have a vector of size graph[current][0]+graph[current][1]+...
  }                                                                             //where the elements are indexes to possible next currents
  if(chooseNext.size() == 0) return 1;                                          //if there is nowhere to go, return with an error
  //cout << "random number: " << rand() % chooseNext.size() << endl;
  current = chooseNext[rand() % chooseNext.size()];                             //set current to the index from our random number landing somewhere in our new vector
  //for(int i=0; i<graph[current].size(); i++){
  //  cout << graph[current][i] << " ";
  //}
  //cout << endl;
  return 0;                                                                     //return success
}
