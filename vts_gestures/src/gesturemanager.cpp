#include "gesturemanager.h"

GestureManager::GestureManager()
{
    iomanager=NULL;
    counterInitMax=2;
    counterEndMax=2;

    counterInit=counterInitMax;

    counterEnd=counterEndMax+1;
    gestureDetected=false;
    state=STATUS_INACTIVE;
}

void GestureManager::updateStatus(FrameData &params)
{

    gestureDetected=isGestureDetected( params );

    if( gestureDetected ){

        resetEndCounter();
        decreaseInitCounter();

    }else{
        resetInitCounter();

        decreaseEndCounter();
    }

    if( gestureDetected ){
        generateGestureData( params );
    }

    if( state==STATUS_INIT ){

        //initializing gesture
        onStatusInit( params );
        //initialized now change to active
        state=STATUS_ACTIVE;
    }else if( state==STATUS_ACTIVE ){
        //progress


        onStatusActive( params );

    }else if( state==STATUS_FINISH ){

        onStatusFinish( params );
        state=STATUS_INACTIVE;
    }else if( state==STATUS_INACTIVE ){

        onStatusInactive( params );
    }

}

void GestureManager::setIOManager( IOEventManager *eventManager)
{
    //iomanager->set( *eventManager );

    //locks the mutex until the end of scope
    boost::lock_guard<boost::mutex> guard(iomanagerMutex);

    //iomanagerMutex.lock();

    if( iomanager!=NULL ){
        delete iomanager;
    }

    iomanager=eventManager;

    //iomanagerMutex.unlock();

}

IOEventManager *GestureManager::getIOManager()
{
    return iomanager;

}

bool GestureManager::isActive() const
{
    return state==STATUS_ACTIVE;
}

int GestureManager::getCounterInitMax() const
{
    return counterInitMax;
}

void GestureManager::setCounterInitMax(int value)
{
    counterInitMax = value;
}

int GestureManager::getCounterEndMax() const
{
    return counterEndMax;
}

void GestureManager::setCounterEndMax(int value)
{
    counterEndMax = value;
}

void GestureManager::decreaseInitCounter()
{

    if( state == STATUS_INACTIVE ){

        //int tmpState=state;
        counterInit--;

        if( counterInit<=0 ){
            state=STATUS_INIT;
        }

    }else if( state==STATUS_INIT){
        state=STATUS_ACTIVE;
    }else{
        //reset when active
        counterInit=counterInitMax;
    }
}

void GestureManager::decreaseEndCounter()
{
    if( state==STATUS_ACTIVE ){
        counterEnd--;

        if( counterEnd<=0 ){
            state=STATUS_FINISH;
        }
    }else if( state==STATUS_FINISH ){
        state=STATUS_INACTIVE;
    }else{
        counterEnd=counterEndMax+1;
    }

}

void GestureManager::resetInitCounter()
{
    counterInit=counterInitMax;

}

void GestureManager::resetEndCounter()
{
    counterEnd=counterEndMax+1;
    /*
    if( state==STATUS_FINISH ){
        state=STATUS_ACTIVE;
    }
    */

}
