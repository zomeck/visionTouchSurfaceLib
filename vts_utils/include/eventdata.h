#ifndef EVENTDATA_H
#define EVENTDATA_H
#include <opencv2/opencv.hpp>
#include <highgui.h>

#include "framedata.h"

using namespace cv;
class EventData
{
public:
    EventData(){
        frameData=NULL;
        mousePosition.x=0;
        mousePosition.y=0;
        offset=0;
    }

    FrameData * frameData;
    Point2i mousePosition;
    int offset;

};

#endif // EVENTDATA_H
