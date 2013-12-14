#include "../include/kinecthandler.h"


int KinectHandler::getResWidth() const
{
    return ResWidth;
}

void KinectHandler::setResWidth(int value)
{
    ResWidth = value;
}

int KinectHandler::getResHeight() const
{
    return ResHeight;
}

void KinectHandler::setResHeight(int value)
{
    ResHeight = value;
}
KinectHandler::KinectHandler()
{
    ResWidth=640;
    ResHeight=480;
}

KinectHandler::~KinectHandler()
{
    context.Shutdown();
}

void KinectHandler::init()
{
    nRetVal = context.Init();
    if (nRetVal != XN_STATUS_OK){
        //print_error("Failed to initialize context: %s\n", xnGetStatusString(nRetVal));
        cerr<<"Failed to initialize context: "<<xnGetStatusString(nRetVal)<<endl;
    }

    // Create a DepthGenerator node
    nRetVal = d_image.Create(context);
    if (nRetVal != XN_STATUS_OK){
        //print_error("Failed to create depth generator: %s\n", xnGetStatusString(nRetVal));
        cerr<<"Failed to create depth generator: "<<xnGetStatusString(nRetVal)<<endl;
    }

    // Create a ImageGenerator node
    nRetVal = g_image.Create(context);
    if (nRetVal != XN_STATUS_OK){
        // print_error("Failed to create image generator: %s\n", xnGetStatusString(nRetVal));
        cerr<<"Failed to create image generator: "<< xnGetStatusString(nRetVal)<<endl;
    }

    // Make it start generating data
    nRetVal = context.StartGeneratingAll();
    if (nRetVal != XN_STATUS_OK){
        //print_error("Failed generating data: %s\n", xnGetStatusString(nRetVal));
        cerr<<"Failed generating data: "<<xnGetStatusString(nRetVal)<<endl;
    }

    //Sync the DepthGenerator with the ImageGenerator
    nRetVal = d_image.GetFrameSyncCap().FrameSyncWith(g_image);
    if (nRetVal != XN_STATUS_OK){
        //print_error("Failed to sync Depth and RGB streams: %s\n", xnGetStatusString(nRetVal));
        cerr<<"Failed to sync Depth and RGB streams: "<<xnGetStatusString(nRetVal)<<endl;
    }

    // Set the view point of the DepthGenerator to match the ImageGenerator
    nRetVal = d_image.GetAlternativeViewPointCap().SetViewPoint(g_image);
    if (nRetVal != XN_STATUS_OK){
        //print_error("Failed to match Depth and RGB points of view: %s\n", xnGetStatusString(nRetVal));
        cerr<<"Failed to match Depth and RGB points of view: "<<xnGetStatusString(nRetVal)<<endl;
    }

    nRetVal = context.WaitAndUpdateAll();
    if (nRetVal != XN_STATUS_OK)
    {
        cerr<<"Failed updating data: %s\n" << xnGetStatusString(nRetVal)<<endl;

    }
}

void KinectHandler::waitFrame()
{
    nRetVal = context.WaitAndUpdateAll();
    if (nRetVal != XN_STATUS_OK)
    {
        cerr<<"Failed updating data: %s\n" << xnGetStatusString(nRetVal)<<endl;

    }

}

void KinectHandler::getDepthMap16(Mat &depthMap)
{

    pDepthMap = d_image.GetDepthMap();
    depthMap = cv::Mat(ResHeight, ResWidth, CV_16UC1, (void*) pDepthMap);

}

void KinectHandler::getDepthMap08(Mat &depthMap)
{

    Mat depths = cv::Mat(ResHeight, ResWidth, CV_16UC1, (void*) pDepthMap);
    depths.convertTo(depthMap, CV_8UC1, 255.0/2048);  //linear interpolation



}

void KinectHandler::getImage(Mat &image)
{
    g_image.GetMetaData (g_imageMD);
    IplImage *rgbimg = cvCreateImageHeader(cvSize(ResWidth,ResHeight), 8, 3);
    pImage = g_image.GetRGB24ImageMap();

    XnUInt8* rgb = g_imageMD.WritableData();
    cvSetData(rgbimg,rgb,rgbimg->widthStep);

    image=cvarrToMat( rgbimg, true );

    cvtColor( image, image, CV_RGB2BGR );


}

void KinectHandler::depthMapCorrection(Mat &input, Mat &output,bool interPolate )
{
    //input.convertTo(output, CV_8UC1, 255.0/2048);

    Mat _tmp,_tmp1; //minimum observed value is ~440. so shift a bit
    Mat( input ).convertTo(_tmp1,CV_64FC1);

    //Point minLoc; double minval,maxval;
    //minMaxLoc(_tmp1, &minval, &maxval, NULL, NULL);
    _tmp1.convertTo(output, CV_8UC1, 255.0/2048);  //linear interpolation

    //use a smaller version of the image
    if ( interPolate ){
        Mat small_depthf;
        resize(output,small_depthf,Size(),0.2,0.2);
        //inpaint only the "unknown" pixels
        cv::inpaint(small_depthf,(small_depthf ==0),_tmp1,5.0,INPAINT_TELEA);
        //output=small_depthf;
        resize(_tmp1, _tmp, output.size());
        _tmp.copyTo(output, (output == 0));  //add the original signal back over the inpaint

    }else{
        cv::inpaint(output,(output ==0),output,5.0,INPAINT_TELEA);
    }


}



