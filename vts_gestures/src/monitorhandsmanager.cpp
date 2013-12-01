#include "monitorhandsmanager.h"

MonitorHandsManager::MonitorHandsManager(): GestureManager()
{
}

bool MonitorHandsManager::isGestureDetected(FrameData &params)
{

    bool movementDetected=params.hands->size() >=1;
    return movementDetected;

}

void MonitorHandsManager::generateGestureData(FrameData &params)
{

}

void MonitorHandsManager::onStatusInit(FrameData &params)
{

    EventData eventParam;
    eventParam.frameData=&params;

    iomanagerMutex.lock();

    iomanager->gestureInitialize( eventParam );

    iomanagerMutex.unlock();

}

void MonitorHandsManager::onStatusActive(FrameData &params)
{

    EventData eventParam;
    eventParam.frameData=&params;

    iomanagerMutex.lock();

    iomanager->gestureInProgressUpdate( eventParam );

    iomanagerMutex.unlock();

}

void MonitorHandsManager::onStatusFinish(FrameData &params)
{

    EventData eventParam;
    eventParam.frameData=&params;

    iomanagerMutex.lock();

    iomanager->gestureInitialize( eventParam );

    iomanagerMutex.unlock();
}

void MonitorHandsManager::onStatusInactive(FrameData &params)
{

}
