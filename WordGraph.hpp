#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class WordGraph{
private:
  vector <string> dictionary;                                                   //vector of words that correspond to the relational graph (sorted)
  vector <vector <int>> graph;                                                  //2d vector of ints: fundamental data structure for unidirectional relational graph
  int current;
public:
  int add(string word);                                                         //attempts to add a new word to the dictionary -- returns 0 if successful, 1 if word already exists
  int add(string word, int index);                                              //overloaded add, for when the caller already knows where word belongs
  int search(string word);                                                      //returns the index in dictionary corresponding to word, or -1 if search was unsuccessful
  int search(string word, int lo, int hi);                                      //overloaded search function, with lower and upper bound for the dictionary search
  int buildGraph();
  int buildGraphVerbose();
  int link(string w1, string w2);                                               //increments the int at w1,w2 -- returns 0 if successful, 1 if one or both words are not found
  int importGraph(string g);                                                    //imports new graph from string using delimeter colon (:) for rows and semicolon (;) for columns
                                                                                //example:                        2  4  8
                                                                                //   "2:4:5;4:0:4;8:4:5"   ->    4  0  4
                                                                                //                                5  4  5
  string exportGraph();                                                         //returns string formatted like above
  int importDictionary(string d);                                               //imports new dictionary from string using delimeter space ( )
                                                                                //example: "cat dog , fish . mouse"
  string exportDictionary();                                                    //returns string formatted like above
  string getCurrent();                                                          //returns the string at dictionary[current]
  int setCurrent(string s);                                                     //searches dictionary for s, if it finds it sets current to index in dictionary, if not adds it to the dictionary and sets current
  int updateCurrent();                                                          //makes a step based on the probabilities and current -- returns 0 if success or 1 if current is null or there are no other links
};
