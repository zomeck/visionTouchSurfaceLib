#ifndef GESTUREMANAGER_H
#define GESTUREMANAGER_H

#include<boost/thread/mutex.hpp>
//#include<boost/thread/lock_guard.hpp>
#include <boost/thread/shared_lock_guard.hpp>


#include "framedata.h"
#include "ioeventmanager.h"

/**
 * @brief The GestureManager class is a pure abstract class that provides an interface to create any gesture and manages its state
 *
 The key idea is that for each frame the controller is going to call updateStatus(). The object will decide then its state and execute the apropiate function.
 For managing the status the class uses counters. If the init counter is set to N the class will switch the state to active when N observations of the gesture
 have been detected. An equivalent process is applied for a counter to end the gesture.
 *
 For making a gesture work it is enough to override the pure virtual functions. Note that each gesture is independent and will not exclude other gestures
 from being performed.
 *
 The GestureManager is associated with an IOEventManager that should be called in the overriden functions accordingly to perform the event.
 */
class GestureManager
{
private:

    int state;
protected:
    int counterInit;
    int counterEnd;


//proably this should be protected and above private...

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

    /**
     * @brief updateStatus processes the data of the given frame
     * @param params data of the frame needed for the gesture
     *
     This function will call GestureManager::generateGestureData, GestureManager::onStatusInit, GestureManager::onStatusActive,
     GestureManager::onStatusFinish or GestureManager::onStatusInactive depending on the output of the function
     GestureManager::isGestureDetected and the values of the counters.
     */
    virtual void updateStatus( FrameData & params );

    /**
     * @brief isGestureDetected determines if the gesture is detected with the given data
     * @param params data of the frame needed for the gesture
     * @return True when the gesture is detected
     */
    virtual bool isGestureDetected( FrameData & params ) = 0;

    /**
     * @brief generateGestureData This function generates the data needed for the gesture manager
     * @param params data of the frame needed for the gesture
     *
     This function is called every time that GestureManager::updateStatus is called and GestureManager::isGestureDetected returns True
     */
    virtual void generateGestureData( FrameData & params ) = 0;

    /**
     * @brief onStatusInit This function is called the first time the init counter reaches 0
     * @param params data of the frame needed for the gesture
     *
     After this function is called the state of the gesture is going to be set as active. The function should not be called again until the
     status of the gesture has gone to inactive again.
     *
     In this function all the atributes that are going to be needed during the gesture should be initialized.
     */
    virtual void onStatusInit( FrameData & params ) = 0;

    /**
     * @brief onStatusActive This function is called while the status is active, until the end counter reaches 0
     * @param params data of the frame needed for the gesture
     *
     Note that the status can be active serveral frames so this function is going to be called several times in a row.
     */
    virtual void onStatusActive( FrameData & params ) = 0;

    /**
     * @brief onStatusFinish This function is called when the end counter reaches 0
     * @param params data of the frame needed for the gesture
     *
     After this function is called the state of the gesutre is going to be set as inactive. The function should not be called again until the
     status of the gesture has gone to active again.
     */
    virtual void onStatusFinish( FrameData & params ) = 0;

    /**
     * @brief onStatusInactive This function is called when the status is set to inactive
     * @param params data of the frame needed for the gestures
     *
     In case a gesture needs passive information this is the function where it should be recorded.
     */
    virtual void onStatusInactive( FrameData & params ) = 0;

    /**
     * @brief setIOManager This function changes the event manager associated with this class
     * @param eventManager pointer to the new event manager
     *
     This function has lateral effects. No copy of the reference is done.
     */
    virtual void setIOManager(IOEventManager *eventManager );

    virtual IOEventManager * getIOManager();

    virtual bool isActive() const;

    int getCounterInitMax() const;
    void setCounterInitMax(int value);
    int getCounterEndMax() const;
    void setCounterEndMax(int value);


};

#endif // GESTUREMANAGER_H

