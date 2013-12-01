#ifndef STOREHANDMASK_H
#define STOREHANDMASK_H
#include <string>
#include<fstream>
#include<iostream>
#include <iomanip>
using namespace std;
#include "ioeventmanager.h"
#include <boost/filesystem.hpp>
class StoreHandMask : public IOEventManager
{

protected:
    virtual void storeHands( EventData & param );
    string folder;
    string fileHeader;
    int offset;
    long counter;
public:
    StoreHandMask();

    void set(const IOEventManager &param );

    void initData();

    void gestureInitialize( EventData & param );

    void gestureInProgressUpdate( EventData & param );

    void gestureFinalize( EventData & param );

    string getFileHeader() const;
    void setFileHeader(const string &value);
    string getFolder() const;
    void setFolder(const string &value);
    int getOffset() const;
    void setOffset(int value);
};

#endif // STOREHANDMASK_H
