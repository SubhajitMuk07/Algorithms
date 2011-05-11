/*
 * main.cpp
 *
 *  Created on: May 11, 2011
 *      Author: kurtisthompson
 */

#include <iostream>
#include "SkipList.cpp"

using namespace std;


int main() {
  SkipList S;
  cout << "Insert 10" << endl;
  S.insert(10);
  cout << "Insert 20" << endl;
  S.insert(20);
  cout << "Insert 30" << endl;
  S.insert(15);
  cout << "Insert 200" << endl;
  S.insert(200);
  cout << "Insert -1" << endl;
  S.insert(-1);
  S.remove(20);
  cout << "Removed 20"<< endl;
  S.insert(40);
  cout << "Inserted 40" << endl;
  cout << S.search(5) << endl;
  cout << S.search(210) << endl;
  cout << S.search(-1) << endl;
  cout << S.search(20) << endl;
  cout << S.search(200) << endl;
  cout << S.search(40) << endl;

  return 0;
}

