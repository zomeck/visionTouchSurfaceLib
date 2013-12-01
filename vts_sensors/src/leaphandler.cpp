#include "../include/leaphandler.h"

LeapHandler::LeapHandler()
{
    depthThresh=20;

    while( !leapSensor.isConnected() ){
        cout<<"Leap connecting..."<<endl;
        sleep(1);
    }
    cout<<"Done!"<<endl;
}

bool LeapHandler::determineTouch(Leap::Vector position)
{

    return true || position[2]>depthThresh && position[2]!=0 ;
}

void LeapHandler::waitFrame()
{

    Leap::Frame frame=leapSensor.frame();
}

bool LeapHandler::findBestMatchOnCurrentFrame(Point2i & inputWorld, float radius, LeapMapping & leap2world, Point3i &match)
{


    Leap::PointableList list;
    list = frame.pointables();
    //cout<<(list[0].tipPosition()[2]<-2)<<endl;

    match.x=-1;
    match.y=-1;
    match.z=-1;


    if( !list.isEmpty() ){
        cout<<"Leap found Pointables: "<<list.count()<<endl;

        //Vector<float> distances;
        float minDist=1e9;
        int minIdx=-1;
        //cout<<"Found "<<list.count()<<" tip(s)."<<endl;
        Leap::Vector leapTipMatch;
        //get (closest) tips' positions from Leap
        for(int i=0;i<list.count();i++)
        {
            Leap::Pointable element=list[i];
            Leap::Vector position =  element.tipPosition();
            Point2i positionLeap;
            positionLeap.x=position[0];
            positionLeap.y=position[1];
            leap2world.mapPoints( positionLeap, positionLeap );
            //leapTouchPts.push_back(Point(position[0],position[1]));

            //converting leap to world

            float distx=inputWorld.x-positionLeap.x;
            float disty=inputWorld.y-positionLeap.y;
            float dist=std::sqrt(distx*distx+disty*disty);

            if(dist<radius && dist<minDist)
            {
                minDist=dist;
                minIdx=i;
                leapTipMatch.x=positionLeap.x;
                leapTipMatch.y=positionLeap.y;
                leapTipMatch.z=position[2];

            }
        }

        bool leapMatchFound=(minIdx!=-1);
        if(leapMatchFound){//point found
            cout<<"Leap best match found "<<match<<endl;
            //cout<<minIdx<<endl;
            //cout<<"Updated touch location: "<<leapTipMatch<<endl;
            //filteredPt=leapTouchPts[minIdx];
            match.x=leapTipMatch[0];
            match.y=leapTipMatch[1];
            match.z=leapTipMatch[2];
            return true;
        }
        else{
            //use Kinect's touch determination
            //return kinectTouchPt;
            return false;
        }
    }else{
        cout<<"Leap list empty"<<endl;
    }

    return false;
}
