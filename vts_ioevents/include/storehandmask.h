#ifndef STOREHANDMASK_H
#define STOREHANDMASK_H
#include <string>
#include<fstream>
#include<iostream>
#include <iomanip>
using namespace std;
#include "ioeventmanager.h"
#include <boost/filesystem.hpp>

/**
 * @brief The StoreHandMask class inherits from IOEventManager to implement an event that stores all hands found in a frame
 *
 This class will store binary images of the hand masks found in the current frame to the folder specified.
 */
class StoreHandMask : public IOEventManager
{

protected:
    virtual void storeHands( EventData & param );
    string folder;
    string fileHeader;
    int offset;
    long counter;
    int waitingFpN;
    int fpNMax;
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
