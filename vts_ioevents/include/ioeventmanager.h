#ifndef IOEVENTMANAGER_H
#define IOEVENTMANAGER_H

#include "eventdata.h"

/**
 * @brief The IOEventManager class is a pure abstract class that provides an interface to create any input/output event called from a GestureManager object
 *
 The idea of this class is to interface to any event using three functions: IOEventManager::gestureInitialize, IOEventManager::gestureInProgressUpdate
 and IOEventManager::gestureFinalize.
 */
class IOEventManager
{
protected:
    bool hasBeenInit;
public:
    IOEventManager();

    virtual void set( const IOEventManager & param ) = 0;

    /**
     * @brief initData This function initializes the atributes needed for using the event later on
     */
    virtual void initData()=0;

    /**
     * @brief gestureInitialize This function is called when the event needs to be started
     * @param param data of the frame needed for the event
     *
     In the example of doing a mouse click and drag this would be the function when the button is clicked.
     In the example of recording a video the file descriptor should be created in this function.
     *
     This function should not be called again until IOEventManager::gestureFinalize has been called.
     */
    virtual void gestureInitialize( EventData & param ) = 0;

    /**
     * @brief gestureInProgressUpdate This function is called when the event is happening
     * @param param data of the frame needed for the event
     *
     This function could be called several times.
     In the example of doing a mouse click and drag this would be the function that updates the mouse position.
     In the exmample of recording a video each call to this function would store a frame.
     */
    virtual void gestureInProgressUpdate( EventData & param )= 0;

    /**
     * @brief gestureFinalize This function is called when the event needs to be finalized
     * @param param data of the frame needed for the event
     *
     This function should not be called again until IOEventManager::gestureInitialize has been called.
     *
     In the example of doing a mouse click and drag this function would unclick the button.
     In the example of recording a video this function should close the file descriptor.
     *
     */
    virtual void gestureFinalize( EventData & param ) = 0;
};

#endif // IOEVENTMANAGER_H
