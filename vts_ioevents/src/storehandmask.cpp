#include "storehandmask.h"


StoreHandMask::StoreHandMask():IOEventManager()
{
    folder.assign("defaultOutput");
    fileHeader.assign("hand");
    offset=10;
    counter=0;
}


void StoreHandMask::storeHands(EventData &param)
{

    //boost::filesystem::path dir(folder);
    //boost::filesystem::create_directory(dir);
    string command("mkdir -p ./");
    command.append( folder );
    system( command.c_str() );


    vector< vector<Point> > contourHandsVect;
    vector<Rect> bboxVect;
    for( int i=0; i<param.frameData->hands->size();i++ ){
        vector<Point> tmp=param.frameData->hands->at(0).contour;
        contourHandsVect.push_back( tmp );
        Rect bbox=boundingRect( param.frameData->hands->at(0).contour );
        bboxVect.push_back( bbox );

    }


    for( int i=0; i<contourHandsVect.size();i++ ){
        Mat tmp=Mat::zeros( param.frameData->depthMap.size(),CV_THRESH_BINARY);
        //cout<< param.frameData->depthMap.size()<<endl;
        drawContours( tmp,contourHandsVect, i,Scalar(255,255,255),CV_FILLED );


        stringstream frameName;

        frameName<<folder<<"/"<<fileHeader<<std::setfill('0')<<std::setw(offset)<<counter<<".jpg";
        counter++;
        imwrite( frameName.str(), tmp );

    }
}
int StoreHandMask::getOffset() const
{
    return offset;
}

void StoreHandMask::setOffset(int value)
{
    offset = value;
}

string StoreHandMask::getFolder() const
{
    return folder;
}

void StoreHandMask::setFolder(const string &value)
{
    folder = value;
}

string StoreHandMask::getFileHeader() const
{
    return fileHeader;
}

void StoreHandMask::setFileHeader(const string &value)
{
    fileHeader = value;
}


void StoreHandMask::set(const IOEventManager &param)
{

}

void StoreHandMask::initData()
{

}

void StoreHandMask::gestureInitialize(EventData &param)
{

    storeHands( param );
}

void StoreHandMask::gestureInProgressUpdate(EventData &param)
{
    storeHands( param );

}

void StoreHandMask::gestureFinalize(EventData &param)
{

    storeHands( param );
}
