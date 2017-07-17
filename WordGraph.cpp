#include "WordGraph.hpp"

int WordGraph::add(string word){
  if(search(word)[0]==-1){                    //if a search for the word comes up empty
    for(int i=0; i<dictionary.size(); i++){   //then we add another row to every column
      graph[i].push_back(0);
    }
    dictionary.push_back(word);               //now we add the word to the dictionary
    vector <int> w;                           //make a new temporary vector for that word's collumn
    for(int i=0; i<dictionary.size(); i++){   //and fill it with zeros
      w.push_back(0);
    }
    graph.push_back(w);                       //now add that collumn to the graph
    return 0;                                 //return 0 indicating success
  }
  return 1;                                   //if the search found something, return 1, indicating failure
}

vector <int> WordGraph::search(string word){
  for(int i=0; i<dictionary.size(); i++){     //search dictionary
    if(dictionary[i].compare(word)==0){       //if it finds a match
      return graph[i];                        //return the column in graph corresponding to word
    }
  }
  vector<int> n = {-1};                       //n stands for null because I'm afraid of keywords
}

int WordGraph::link(string w1, string w2){
  int iw1=-1;                                 //iw1 stands for index of word 1
  int iw2=-1;                                 //I think you can guess what iw2 stands for
  for(int i=0; i<dictionary.size(); i++){     //search dictionary
    if(dictionary[i].compare(w1)==0){         //if it finds a match
      iw1 = i;                                //set iw to index where we found it
      break;                                  //no need to finish the traversal
    }
  }
  if(iw1==-1) return 1;                       //if iw was never overwritten, w wasn't found and link failure
  for(int i=0; i<dictionary.size(); i++){     //search dictionary
    if(dictionary[i].compare(w2)==0){         //if it finds a match
      iw2 = i;                                //set iw to index where we found it
      break;                                  //no need to finish the traversal
    }
  }
  if(iw2==-1) return 1;                       //if iw was never overwritten, w wasn't found and link failure
  graph[iw1][iw2]++;                          //increment that spot on graph by one to link w1 to w2
  return 0;
}

int WordGraph::importGraph(string g){         //all the ways g can be deformed:
                                              //unequal lengths between semicolons
                                              //number of collumns and rows not equal
                                              //invalid characters
  int graphSize = -1;
  int rowCounter = 0;
  int collumnCounter = 0;
  if(g[0]==';' || g[0]==':' || g[g.size()-1]==';' || g[g.size()-1]==':') return 1;
  for(int i=0; i<g.size(); i++){
    if(g[i]<'0' || g[i]>';') return 2;        //invalid tokens
    else if(g[i]==';'){
      if(graphSize==-1) graphSize = rowCounter+1;  //setting graph size based on the number of rows in first collumn
      if(rowCounter<graphSize-1) return 1;    //at least one row is too small
      rowCounter = 0;
      collumnCounter++;
    }
    else if(g[i]==':'){
      rowCounter++;
      if(graphSize!=-1 && rowCounter>graphSize-1) return 1; //at least one row is too big
    }
  }
  if(graphSize == -1) return 1;               //malformed input
  if(collumnCounter != graphSize-1) return 1; //there should be an equal number of collumns and rows
                                              //now that we have completed the input data validation, we can move on to the actual algorithm
  graph.clear();                              //clear the old graph to make room for the new one
  int gin = 0;                                //the current index of our manual traversal
  int glen = 1;                               //the number of digits in the urrent number in our traversal
  vector<int> temp;                           //temporary vector to build our collumns/rows
  for(int i=0; i<graphSize; i++){             //collumn traversal
    for(int j=0; j<graphSize; j++){           //row traversal
      while(gin+glen<g.size() && g[gin+glen]<':'){
        glen++;                               //as long as the next token isn't a delim, keep adding to the length
      }
      temp.push_back(atoi((g.substr(gin, glen)).c_str()));  //we are turning a string into a substring into a c-string into an int
      gin += glen+1;                          //add the length to the current index plus one to hop onto the first digit of the next number
      glen = 1;                               //reset the length for the next loop iteration
    }
    graph.push_back(temp);                    //add temp to the graph (it is a new collumn)
    temp.clear();                             //clear out temp for the new loop traversal
  }
  return 0;                                   //finally we return a success (hopefully)
}

string WordGraph::exportGraph(){
  string out = "";                            //start with an empty string
  for(int i=0; i<graph.size(); i++){          //traverse collumns
    for(int j=0; j<graph[i].size(); j++){     //traverse rows
      out += to_string(graph[i][j]);          //add number to the output after converting to string
      if(j+1<graph[i].size()){                //if this isn't the last number in the collumn
        out += ':';                           //add in a colon as delim
      }
    }
    if(i+1<graph.size()){                     //if this isn't the last collumn in the graph
      out += ';';                             //add semicolon as delim
    }
  }
  return out;                                 //return that whole string
}

int WordGraph::importDictionary(string d){
  if(d[0]==' ' || d[d.size()-1]==' ') return 1; //can't have a delim at the start or end
  dictionary.clear();                         //clear out the dictionary so that we don't append
  int din = 0;                                //same concept as in import graph...
  int dlen = 1;
  while(din < d.size()){
    while(din+dlen<d.size() && d[din+dlen]!=' '){
      dlen++;                                 //add to length as long as it's not a delim (making sure not to go out of bounds)
    }
    dictionary.push_back(d.substr(din,dlen)); //push the substring to the end of dictionary
    din += dlen+1;
    dlen = 1;
  }
  return 0;                                   //return success
}

string WordGraph::exportDictionary(){
  string out = "";                            //start with an empty string
  for(int i=0; i<dictionary.size(); i++){     //traverse the dictionary
    out += dictionary[i];                     //add current word to output string
    if(i+1<dictionary.size()) out+=' ';       //for all except last word, add space as delim after
  }
  return out;                                 //return our output string
}
