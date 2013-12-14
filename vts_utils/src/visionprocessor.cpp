#include "visionprocessor.h"


ProjectiveMapping VisionProcessor::getKinect2screen() const
{
    return kinect2screen;
}

void VisionProcessor::setKinect2screen(const ProjectiveMapping &value)
{
    kinect2screen = value;
}

int VisionProcessor::segmentScreen(const Mat &grayFrame, int Ith, float epsilon, vector<Point2i> &screenContours, Mat &maskShow)
{
    //segment screen
    Mat IThres;
    vector<vector<Point2i> > contours;
    Canny( grayFrame, IThres, Ith, Ith, 3 );

    findContours( IThres, contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );

    float areaMax=0,area;
    int idContourMax=-1;
    for( int i=0; i<contours.size(); i++ ){
        area=contourArea( contours[i] );
        if( area>areaMax ){
            areaMax=area;
            idContourMax=i;
        }
    }
    //cout<<contours.size()<<" "<<idContourMax<<endl;

    //Mat drawing = Mat::zeros( IThres.size(), CV_8UC3 );
    Mat mask = Mat::zeros(IThres.size(), CV_8UC1);
    //Mat maskShow;//= Mat::zeros(IThres.size(), CV_8UC3);
    if( idContourMax>=0 ){
        //cout<<contours[idContourMax].size()<<endl;
        drawContours(mask, contours, idContourMax, Scalar(255), CV_FILLED);
        //drawContours(drawing,contours,idContourMax,Scalar(0,255,0));

        approxPolyDP(contours[idContourMax],screenContours,epsilon,true);

        //cout<<contoursFiltered.size()<<endl;
        //mask.copyTo(maskShow);

        //to show aproximation diferences
        vector<Mat> maskvect;
        maskvect.push_back(mask);
        maskvect.push_back(mask);
        maskvect.push_back(mask);
        merge(maskvect,maskShow);

        vector<vector<Point2i> > tmpVect;
        tmpVect.push_back(screenContours);

        if( screenContours.size()==4 ){
            drawContours(maskShow, tmpVect, 0, Scalar(0,255,0),3 );
        }else{
            drawContours(maskShow, tmpVect, 0, Scalar(0,0,255) );
        }



    }

    return idContourMax>=0;
}

VisionProcessor::VisionProcessor()
{
    wResolution=1024;
    hResolution=768;

    //verboseVideo=false;
}

void VisionProcessor::init(int wRes, int hRes)
{
    kinect.init();
    wResolution=wRes;
    hResolution=hRes;


    //creating mask params
    thicknessHand = 15;//2.5
    thicknesFinger = 2;

    //hand area size boundaries
    //sizes=new int[2];
    //sizes[1]=640;
    //sizes[0]=480;

    //for creating cv::mats
    sizes[1]=kinect.getResWidth();
    sizes[0]=kinect.getResHeight();

    minAreaContour=850;
    maxAreaContour=(sizes[0]*sizes[1]/16);
    //hand aprox polyfit error allowed
    tolerance=1.5;//hand smoothing tolerance

    //fingertip parameters
    maxAreaFinger=350;
    minAreaFinger=50;
    maxExcentricity=1;
    minExcentricity=0.35;
    maxCirc=2.5;


    //hand parameters
    maxDist2Class=0.4;//0.4
    descriptorNum=15;//25
    minHandExc=0.37;

    //screen corners params
    epsilon=6;


    errorTolerance=epsilon;

    erodeK=getStructuringElement(MORPH_ERODE,Size(3,3) );
    useScreenSeg=true;
    //histogram params
    //int numFrames=200;
    //bool useHist=true;

    //use only screen area
    //bool useScreenSeg=true;

    //uncertanty of kinect
    //float radiusUncertantyKinect=50;

    //FourierDescriptor dictionary( std::string( "dictionary.txt" ) );
    fdDictionary.loadDictionary( std::string( "dictionaryFD25_hands1_leaning0_ext.txt" ) );



    //delete timeinfo;

}

void VisionProcessor::getScreenTransformation()
{
    //segmenting screen
    vector<Point2i> screenContours;
    Mat maskShow, colorFrame,grayFrame;

    namedWindow( "ScreenSegmentation", CV_WINDOW_FREERATIO );


    int Ith=140;//70 with les light
    cvCreateTrackbar("IThreshold","ScreenSegmentation",&Ith,255,0);

    //cvCreateTrackbar("Tolerance","ScreenSegmentation",&errorTolerance,10,0);
    vector<Mat> bgrChannels;
    vector< vector<Point2i> > contoursVect;
    while(true){
        kinect.waitFrame();
        kinect.getImage( colorFrame );

        //cvtColor( colorFrame, hsvFrame, CV_BGR2HSV_FULL );
        //split(hsvFrame ,hsvChannels );
        cvtColor(colorFrame,grayFrame,CV_BGR2GRAY);

        split( colorFrame, bgrChannels );
        segmentScreen( grayFrame, Ith, errorTolerance, screenContours,maskShow );

        mask = Mat::zeros( colorFrame.size(), CV_8UC1);

        contoursVect.clear();
        contoursVect.push_back(screenContours);

        drawContours( mask, contoursVect, 0, Scalar(255), CV_FILLED );
        Mat dilateK=getStructuringElement( MORPH_DILATE, Point(100,100) );
        dilate( mask, mask, dilateK );
        //cout<<screenContours.size()<<endl;


        imshow("ScreenSegmentation", maskShow );

        //usleep(100*1000);

        char key =cvWaitKey(900);
        if (key==13/*Enter*/) {
            key=cvWaitKey( 0 );
            if(key==13){
                break;
            }
        }
    }
    cout<<"size found "<<screenContours.size() <<endl;



    //should not be necessesary at all
    screenContoursCorners.clear();

    for( int i=0; i < screenContours.size(); i++ ){

        Point2f tmp;
        tmp.x=screenContours.at(i).x;
        tmp.y=screenContours.at(i).y;
        screenContoursCorners.push_back( tmp );

    }

    cout<< screenContoursCorners<<endl;
    //char key =cvWaitKey(0);


    ProjectiveMapping tmp( wResolution, hResolution );
    kinect2screen=tmp;
    kinect2screen.calculateMapping( screenContoursCorners );

}

void VisionProcessor::createMaxDepthWithHistogram(int numFrames)
{
    int minBinBound=-255;
    int maxBinBound=-minBinBound;
    int numBins=abs(minBinBound)+abs(maxBinBound)+1;

    int threshold=0.01*numFrames; //min depth that gives at least this value in hist is d_max

    //Mat kinectImg(5, 5, CV_32S);
    //randu(kinectImg, Scalar::all(100), Scalar::all(255));
    Mat surfaceDepth;
    kinect.waitFrame();
    kinect.getDepthMap16( surfaceDepth );
    KinectHandler::depthMapCorrection( surfaceDepth, surfaceDepth );

    Mat kinectImg=surfaceDepth.clone();
    //cout<<kinectImg<<endl;


    int size_histogram[]={kinectImg.rows, kinectImg.cols, numBins};

    Mat temporalHistogram(3, size_histogram,CV_32S, Scalar::all(0));

    for(int t=0; t<numFrames; t++) //at each time step
    {
        //read frame
        //Mat frame = Mat(kinectImg.rows, kinectImg.cols, CV_32S);
        //randu(frame, Scalar::all(0), Scalar::all(255));

        Mat frame;
        kinect.waitFrame();
        kinect.getDepthMap16( frame );
        KinectHandler::depthMapCorrection( frame, frame );

        Mat frameS, kinectImgS;
        frame.convertTo( frameS, CV_32S );
        kinectImg.convertTo( kinectImgS, CV_32S );
        Mat diff=frameS-kinectImgS;
        //cout<<diff<<endl;
        //for each pixel
        for(int i=0; i<kinectImg.rows; i++)
        {
            for(int j=0; j<kinectImg.cols; j++)
            {
                int temp=diff.at<int>(i,j)-minBinBound;
                //cout<<diff.at<int>(i,j)<<"\t"<<temp<<endl;
                temporalHistogram.at<int>(i,j,temp)+=1;

            }
        }
    }

    //cout << temporalHistogram.at<int>(0,0,255)<< endl;
    Mat d_max=kinectImg.clone();

    //for each pixel
    for(int i=0; i<kinectImg.rows; i++)
    {
        for(int j=0; j<kinectImg.cols; j++)
        {
            //find d_max (starting search at closest pixels)
            int idx=minBinBound;
            int cumulativeHist=0;
            bool d_maxFound=false;
            while(idx<maxBinBound && !d_maxFound)
            {

                int diffVal=temporalHistogram.at<int>(i,j,idx-minBinBound);

                //cout<<diffVal<<" ";

                cumulativeHist+=diffVal;
                d_maxFound=(cumulativeHist>threshold);
                if( d_maxFound){
                    d_max.at<u_int8_t>(i,j)=kinectImg.at<u_int8_t>(i,j)+idx;
                    //cout<<kinectImg.at<int>(i,j);
                    //cout<<endl<<idx<<endl;

                }
                idx++;
                //cout<<idx<<endl;
            }
        }
    }

    maxDepth= d_max.clone()-1;

    minDepthHand = maxDepth - thicknessHand;
    minDepthFinger = maxDepth - thicknesFinger;
}

void VisionProcessor::processFrame(FrameData &frameData, double &processingDuration)
{
    time_t start,end;
    frameData.kinect2screen=&kinect2screen;

    // Wait for new data to be available
    kinect.waitFrame();

    time(&start);

    /*
    if( useLeap ){
    leapHandler->waitFrame();
    }
    */
    Mat depth16,colorFrame,grayFrame,touchHand,touchFinger,depth8,touchFingerCopy,touchHandCopy,depth;
    // Take current depth map with 16 bits int
    kinect.getDepthMap16( depth16 );
    // Take current depth map with 8 bits int
    //kinect.getDepthMap08( depth );
    // Take current color image
    kinect.getImage( colorFrame );

    depth16.convertTo(depth8, CV_8UC1, 255.0/2048);



    cvtColor(colorFrame,grayFrame,CV_BGR2GRAY);

    touchHand =cv::Mat::zeros( depth8.size(), CV_8UC1 );


    depth8.copyTo( depth, mask );
    frameData.depthMap=depth.clone();

    //imshow("depth",depth);

    if( useScreenSeg ){
        depth.copyTo( touchHand,mask );
        depth.copyTo( touchFinger,mask );
    }else{
        depth.copyTo( touchHand );
        depth.copyTo( touchFinger );

    }



    inRange(touchHand,minDepthHand,maxDepth,touchHand);//SEGMENTING hand
    erode(touchHand,touchHand,erodeK);
    //erode(touchHand,touchHand,erodeK);

    inRange( touchFinger, minDepthFinger, maxDepth, touchFinger );//SEGMENTING finger
    //erode(touchFinger,touchFinger,erodeK);

    touchFinger.copyTo( touchFingerCopy );
    //bitwise_and( prevTouchFinger, touchFinger, touchFinger );//temporal filtering

    touchHand.copyTo(touchHandCopy);
    //bitwise_and(prevTouchHand,touchHand,touchHand);//temporal filtering

    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Mat touchHandCopy2=touchHand.clone();

    findContours( touchHandCopy2, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );

    //=============
    //Finding hands
    //=============

    //Mat drawing = Mat::zeros( touchCopy.size(), CV_8UC3 );
    frameData.rgbDebug = colorFrame.clone();
    frameData.handsMaskRaw=touchHand.clone();
    frameData.fingersMaskRaw=touchFinger.clone();

    vector< vector<Point> > contoursHandFiltered;
    //vector<Hand> hands;
    contoursHandFiltered.clear();
    //vector< Mat > contourMaskVect;
    //std::string normUsed("1norm");
    std::string normUsed("1norm");


    for( int i = 0; i< contours.size(); i++ )
    {
        float area=contourArea( contours.at(i) );
        double distanceToClass;

        RotatedRect bBoxf=minAreaRect( contours.at(i) );
        double excHand=((float)std::min(bBoxf.boundingRect().width, bBoxf.boundingRect().height))/
                ((float)std::max(bBoxf.boundingRect().width,bBoxf.boundingRect().height));

        if( area>minAreaContour && area < maxAreaContour && excHand>minHandExc &&
                fdDictionary.findMatchInDictionary( contours.at(i), normUsed,distanceToClass,descriptorNum)==HAND_CLASS_ID && distanceToClass<=maxDist2Class ){

            //cout<<"Distance "<<distanceToClass<<endl;
            //cout<<"Excentricity "<<excHand<<endl;
            Scalar color = Scalar( 255, 0, 0 );
            vector<Point> tmpHandCVect;
            approxPolyDP( contours.at(i),tmpHandCVect , tolerance, true );
            contoursHandFiltered.push_back( tmpHandCVect );

            Hand hand;
            hand.shapeType=-1;
            hand.contour=tmpHandCVect;


            drawContours( frameData.rgbDebug , contoursHandFiltered, contoursHandFiltered.size()-1, color,2);//, hierarchy);//, 0, Point() );

            Mat handMask=Mat::zeros( grayFrame.size(), CV_THRESH_BINARY);
            drawContours( handMask,contoursHandFiltered, contoursHandFiltered.size()-1,Scalar(255,255,255),CV_FILLED );


            float areaReg=1000;
            float minAreaReg=150;
            bool stop=false;
            while( !stop ){
                Mat handMaskCpy=handMask.clone();
                vector< vector<Point> >contourHandMask;
                vector<Vec4i> hierarchyHand;
                findContours(handMaskCpy, contourHandMask, hierarchyHand,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE, Point(0,0) );

                areaReg=contourArea( contourHandMask.at(0) );

                stop= areaReg<=minAreaReg;
                if( !stop ){
                    RotatedRect minbox=minAreaRect( contourHandMask.at(0) );

                    float kernelRadius=std::min( minbox.size.width, minbox.size.height )/8;
                    kernelRadius=std::max( kernelRadius, 3.0f );

                    Mat erodeMag=getStructuringElement(MORPH_ERODE,Size(kernelRadius,kernelRadius) );

                    erode( handMask, handMask, erodeMag );
                }

            }

            vector< vector<Point> >contoursCentroid;
            vector<Vec4i> hierarchyHand;

            findContours( handMask, contoursCentroid,hierarchyHand,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE, Point(0,0) );

            int idxMax=-1;
            float maxArea=0;

            for( int k=0; k<contoursCentroid.size();k++ ){
                float area=contourArea( contoursCentroid.at(k) );

                if( area>maxArea ){
                    idxMax=k;
                    maxArea=area;
                }
            }
            Point2f centroidHand;
            if( idxMax!=-1 ){
                RotatedRect bboxCentroid=minAreaRect( contoursCentroid.at( idxMax ) );

                circle( frameData.rgbDebug , bboxCentroid.center, 5, Scalar(0,0,255),3,CV_FILLED );
                centroidHand=bboxCentroid.center;
            }else{
                RotatedRect bboxCentroid=minAreaRect( hand.contour );
                centroidHand=bboxCentroid.center;

            }



            //imshow("handEroded", handMask);

            hand.centroid=centroidHand;
            frameData.hands->push_back( hand );

            //cout<<"YES-Distance to class"<<distanceToClass<<endl;

        }else{
            //drawContours( frameData.rgbDebug, contours,i,Scalar(0,0,255),2);
            //cout<<"NO-Distance to class"<<distanceToClass<<endl;
        }




    }

    //Mat handMask=depth.clone();
    //frameData.contourHandsFiltered=&contoursHandFiltered;

    //===========================
    //Finding fingertips of hands
    //===========================

    // vector< handFingertipsContour >, handFingertipsContour=vector<vector<point>>
    vector< vector< vector< Point > > > contoursFingersHands;
    vector< bool >foundPalmHands;

    for( int i=0; i< contoursHandFiltered.size(); i++ ){

        Mat handMask=Mat::zeros(depth8.rows,depth8.cols, cv::THRESH_BINARY );
        drawContours(handMask,contoursHandFiltered,i,Scalar(255,255,255),CV_FILLED);

        Mat touchFingerHand_i=touchFinger.clone();
        if( useScreenSeg ){
            bitwise_and(handMask,touchFingerHand_i,touchFingerHand_i,mask);
        }else{
            bitwise_and(handMask,touchFingerHand_i,touchFingerHand_i );
        }

        vector< vector<Point> > contoursF,contoursFFiltered;
        vector<Vec4i> hierarchyF;

        Mat touchFingerC= touchFingerHand_i.clone();
        findContours( touchFingerC, contoursF, hierarchyF, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        //int minExc=1;
        //int minIdx=-1;

        float exc;
        bool foundPalm=false;
        for( int j=0; j<contoursF.size(); j++ ){
            RotatedRect bBoxf=minAreaRect( contoursF.at(j) );
            exc=((float)std::min(bBoxf.boundingRect().width, bBoxf.boundingRect().height))/
                    ((float)std::max(bBoxf.boundingRect().width,bBoxf.boundingRect().height));
            float areaF= contourArea( contoursF.at(j) );
            float perimF= arcLength( contoursF.at(j), true );

            float circ;
            //calculating circularity, circle would give aprox 0
            circ= perimF*perimF/( 4*PI*areaF ) -1;



            foundPalm=foundPalm || areaF>maxAreaFinger;

            if( exc<=maxExcentricity && exc>=minExcentricity && areaF>=minAreaFinger && areaF<=maxAreaFinger && circ<maxCirc ){

                //cout <<"Excentricity "<<exc<<endl;
                contoursFFiltered.push_back( contoursF.at(j) );
                RotatedRect fBB=minAreaRect( contoursF.at(j) );
                /*
                Point2f rect_points[4]; fBB.points( rect_points );
                for( int j = 0; j < 4; j++ ){
                    line( drawingHand, rect_points[j], rect_points[(j+1)%4], Scalar(0,255,0), 2, 8 );
                }
                */
                circle( frameData.rgbDebug ,fBB.center,2,Scalar(0,255,0),2);
                //drawContours( drawingHand, contoursFFiltered, contoursFFiltered.size()-1,Scalar(0,0,255),CV_FILLED );

                drawContours( frameData.rgbDebug, contoursF, j, Scalar(0,255,255), 1 );
                Finger finger;
                finger.contour=contoursF.at(j) ;
                frameData.hands->at(i).fingers.push_back( finger );
            }

        }

        contoursFingersHands.push_back( contoursFFiltered );
        foundPalmHands.push_back( foundPalm );
        //hands.at(i).fingers=contoursFFiltered;
        //hands.at(i).fingers.insert()
        //copy( contoursFFiltered.begin(), contoursFFiltered.end(), hands.at(i).fingers.be)
        frameData.hands->at(i).foundPalm=foundPalm;

    }


    time(&end);
    processingDuration=difftime(end,start);

}

Size2i VisionProcessor::getKinectResolution() const
{

    return Size2i( sizes[1],sizes[0]);
}
