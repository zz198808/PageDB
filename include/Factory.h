#ifndef _FAC_H
#define _FAC_H

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "Log.h"
#include "Slice.h"

class Factory
{
public:
    Factory() { log = Log::GetInstance(); }
    virtual ~Factory() {}

public:
	/**Pure Virtual Function ??? why error, can't allocate enough spaces**/
    virtual bool   put(const Slice & key,const Slice & value) = 0;
    virtual Slice  get(const Slice & key) = 0;
    virtual bool   remove(const Slice & key) = 0;
    virtual bool   init(const char * filename) = 0;

protected:
	Log  *  log;
};

#endif