#ifndef GESTUREMANAGER_H
#define GESTUREMANAGER_H

#include<boost/thread/mutex.hpp>
//#include<boost/thread/lock_guard.hpp>
#include <boost/thread/shared_lock_guard.hpp>


#include "framedata.h"
#include "ioeventmanager.h"
class GestureManager
{
private:

    int state;
protected:
    int counterInit;
    int counterEnd;


//proably this protected and upper private...

    int counterInitMax;
    int counterEndMax;

    static const int STATUS_ACTIVE=0;
    static const int STATUS_INACTIVE=-2;
    static const int STATUS_INIT=-1;
    static const int STATUS_FINISH=1;

    IOEventManager * iomanager;
    bool gestureDetected;

    boost::mutex iomanagerMutex;

    virtual void resetInitCounter();
    virtual void resetEndCounter();
private:


    virtual void decreaseInitCounter();
    virtual void decreaseEndCounter();

    //EventData eventData;
public:
    GestureManager();
    ~GestureManager(){
        if( iomanager!=NULL ){
            delete iomanager;
        }
    }

    virtual void updateStatus( FrameData & params );

    virtual bool isGestureDetected( FrameData & params ) = 0;

    virtual void generateGestureData( FrameData & params ) = 0;

    virtual void onStatusInit( FrameData & params ) = 0;

    virtual void onStatusActive( FrameData & params ) = 0;

    virtual void onStatusFinish( FrameData & params ) = 0;

    virtual void onStatusInactive( FrameData & params ) = 0;

    virtual void setIOManager(IOEventManager *eventManager );

    virtual bool isActive() const;

    int getCounterInitMax() const;
    void setCounterInitMax(int value);
    int getCounterEndMax() const;
    void setCounterEndMax(int value);


};

#endif // GESTUREMANAGER_H

