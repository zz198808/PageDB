#ifndef _E_TABLE_H
#define _E_TABLE_H
#define PAGESIZE 25

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

#include <sys/types.h>
#include "Factory.h"
#include "Slice.h"

#define PAGESIZE 25
#define SINT     sizeof(int)
#define SOFF     sizeof(off_t)
#define SPAGE    sizeof(Page)
#define SEBLOCK  sizeof(EmptyBlock)

typedef int (*HASH)(const string&key);

extern int defaultHashFunc(const string&str);

typedef struct _tEmptyEle{
  int   pos, size;
  _tEmptyEle()
  { pos = size = -1;  }
}EmptyEle;

class EmptyBlock{
public:
    EmptyBlock();
  ~ EmptyBlock() {};
private:
  int      curPos;
  int      nextBlock;
  EmptyEle eles[PAGESIZE];
};

typedef struct {
  int   hash_value;
  int   data_pointer; 
  int   key_size;   
  int   data_size;
} PageElement;

class ExtendibleHash;

class Page{
public:
	Page(ExtendibleHash * eHash);
  ~ Page();

private:	
    bool   full();
    bool   put(const string&key,const string&value, int hashVal);
    string get(const string&key, int hashVal);
    bool   remove(const string&key, int hashVal);
private:
    friend class ExtendibleHash;
    ExtendibleHash * eHash;
private:
  /**
     Asscoiate with the file, 
     Use PAGESIZE + 5 to avoid some special situation
  **/
	int d, curNum;
  PageElement elements[PAGESIZE + 5];
};

class ExtendibleHash : public Factory{
public:
	  ExtendibleHash(HASH hashFunc = defaultHashFunc);
  ~ ExtendibleHash();

public:
  	bool     put(const string&key,const string&value);
  	string   get(const string&key);
    bool     remove(const string&key);
    bool     init(const string&filename);

private:
    void         writeToFile();
    void         readFromFile();
    EmptyBlock   findSuitable(int size, int & addr, int & pos, int & datAddr);

private:
  HASH           hashFunc;
  Page   *       page;
  int            curId;
  fstream        idxfs;
  fstream        datfs;
  friend  class  Page;

private:
  /**Need read from file**/
  int            gd, pn;
	vector <int>   entries;
  EmptyBlock     block;
};

#endif