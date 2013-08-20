#include <iostream>
#include <iomanip>
//if windows and specified os
#if (defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || (defined(__APPLE__) & defined(__MACH__)))
#include <cv.h>
#include <highgui.h>
#else//otherwise...
#include <opencv/cv.h>
#include <opencv/highgui.h>
#endif

#include <cvblob.h>
#include "functions.h"
#include <stdio.h>
using namespace cvb;
using namespace std;

int main()
{
  CvTracks tracks; //declaring structure for the list of tracks..
   
  cvNamedWindow("red_object_tracking", CV_WINDOW_AUTOSIZE); //window
  cvMoveWindow("red_object_tracking", 0,0);

  CvCapture *capture = cvCaptureFromCAM(0); //getting video from camera to videocapture structure...
//baically it is uses to grab frame fast important for sync.
  cvGrabFrame(capture); // from camera or avi...
  IplImage *img = cvRetrieveFrame(capture); //following cvRetrieveFrame function which gets image grabbed by the  cvGrabFrame and pointers to that image...
   //it stores the image size...
  CvSize imgSize = cvGetSize(img);
   // structure to hold the image's param.
  IplImage *frame = cvCreateImage(imgSize, img->depth, img->nChannels);
   // used for the structuring element consists of columns,rows and anchor pixel with the shape...
  IplConvKernel* morphKernel = cvCreateStructuringElementEx(5, 5, 1, 1, CV_SHAPE_RECT, NULL);

  //unsigned int frameNumber = 0;
  unsigned int blobNumber = 0;

  bool quit = false; 
  long size_s=0;
    
  // looping for the grabbing the frame...
  while (!quit && cvGrabFrame(capture))
  {
    IplImage *img = cvRetrieveFrame(capture); //different block hence again declaring the image...to store the grabbed the frame..

    cvConvertScale(img, frame, 1, 0);//converts the src to the dest with some modification...
    cvFlip(frame, frame, 1);
    IplImage *segmentated = cvCreateImage(imgSize, 8, 1);// another image structure with params--> imgSize, 8 bit-depth, 1 channel..
    
    // Detecting red pixels:
    // (This is very slow, use direct access better...)
    //two loops are put into this...from the upper leftmost pixel...
    for (unsigned int j=0; j<imgSize.height; j++)
      for (unsigned int i=0; i<imgSize.width; i++)
      {
	CvScalar c = cvGet2D(frame, j, i);//gettin the image info for every point in the image...
        //setting the RGB value for every point...               
	double b = ((double)c.val[0])/255.; //  deriving the RGB value...
	double g = ((double)c.val[1])/255.; //
	double r = ((double)c.val[2])/255.; //
	unsigned char f = 255*(((r>0.3+g)&&(r>0.3+b))||((b>.2+g)&&(b>.2+r))||((g>.8+r)&&(g>.8+b))); // checkin that if red is greater than other components (green, blue)...
	//now for blue color...
	//unsigned char f1= 255*((b>.2+g)&&(b>.2+r));
	cvSet2D(segmentated, j, i, CV_RGB(f, f, f)); //here it sets the value of the BGR into the image structure "segmentated"... 
        //cvSet2D(segmentated, j, i, CV_RGB(f1, f1, f1));
      } 
    cvMorphologyEx(segmentated, segmentated, NULL, morphKernel, CV_MOP_OPEN, 1);//opening of the image...only one time...

    //cvShowImage("segmentated", segmentated);
    cout<<"width-"<<imgSize.width<<endl;
    cout<<"height-"<<imgSize.height<<endl;

    IplImage *labelImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_LABEL, 1); //another image with 1 channel and the same size..
   //------------------------------------------------------------------
    CvBlobs blobs; // structure variable of CvBlobs type...to contain list
    CvBlob* blob;
    CvBlob* blob2;
    // a new structure now to contain blob...	
    //CvBlob* blob;
    unsigned int result = cvLabel(segmentated, labelImg, blobs); //get numbr f labeled pixels..arg is input image, output image and list of blobs...
    cvFilterByArea(blobs, 500, 1000000);// filter blobs by area...if nt in range blobs get erased...
    // renders blob onto the image with a box bounding the blob area...with label shown
    cvRenderBlobs(labelImg, blobs, frame, frame, CV_BLOB_RENDER_BOUNDING_BOX);
    int x1,y1;
    int x;
    int y;
    int x2,y2;
    
    cvUpdateTracks(blobs, tracks, 200., 5); // updates the track... wid theInactive= 200 and theActive frames...
    // if blobs inActive frames less than the Active frames then the track is deleted...
    cvRenderTracks(tracks, frame, frame, CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX);
    //prints tracks info....
    cvShowImage("red_object_tracking", frame); //it displays image in specified window...
     
    /*std::stringstream filename;
    filename << "redobject_" << std::setw(5) << std::setfill('0') << frameNumber << ".png";
    cvSaveImage(filename.str().c_str(), frame);*/
    //blob = blobs[cvGreaterBlob(blobs)];
    //here code for the centroid ...
    //x = blob->m10/blob->area;
    //y = blob->m01/blob->area;
    
    CvBlobs::const_iterator it=blobs.begin();
    //blob=(CvBlob *)malloc(1000*sizeof(CvBlob));
    cout<<"BLOB - "<<&blob<<endl;
    blob=it->second;
    //blob= blobs[cvGreaterBlob(blobs)];
    cout << "Blob #" << blob->label << endl;
    cout << " - Centroid     = (" << blob->centroid.x << ", " << blob->centroid.y << ")" << endl;
    x1=(blob->centroid.x)*(1366/300);
    y1=(blob->centroid.y)*(768/200);
    //mouse(blob->centroid.x, blob->centroid.y);    
    //-------------------------------------------------------
    // Segmentation fault in the function below...
    mouse(x1,y1);
    //delete blob;
    for(CvBlobs::const_iterator it1=blobs.begin(); it1!= blobs.end(); it1++)
    {
    	if(((it1->second)->label)==2) 
    		blob2=it1->second;
    }
    x2=((blob2->centroid).x)*(1500/640);
    y2=((blob2->centroid).y)*(850/480);
    
    int a= fabs(x1-x2);
    int b= fabs(y1-y2);
    
    int d1= a*a;
    int d2= b*b;
    
    float d= sqrt(d1+d2);
    
    cout<<"DISTANCE- "<<d<<endl;
    
    if(d<300)
    {
    	mouseClick(x1, y1);
    }  		
    
    cout<<"working at segmented and labelImg"<<segmentated<<" "<<labelImg<<endl;   
    cout<<"SIZES are"<<size_s<<endl; 
    size_s += sizeof(segmentated);
/*    
    IplImage **a=&labelImg, **b=&segmentated;
    
    free(*a);
    free(*b);  
    
*/
    //cvReleaseBlob(blob);
    cvReleaseImage(&labelImg);// releases the pointer to the structure...
    cvReleaseImage(&segmentated);//again the same...
    
    cout<<"AFTER working at segmented and labelImg"<<segmentated<<" "<<labelImg<<endl;   
   // cout<<"blob"<<&blob;
    char k=cvWaitKey(10)&0xff; // waits for 10 secs...basically returns the key pressed...
    switch (k) // checking the switch cases...
    {
      case 27:
      case 'q':
      case 'Q':
        quit = true;
        break;// it quits...
      /*case 's':
      case 'S':
        for (CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it) // for loop..initialize with bloblist beginning...goes till the end of the list...
        {
          
          //std::stringstream filename; //declaring stringstream...using for the images name
          //filename << "redobject_blob_" << std::setw(5) << std::setfill('0') << blobNumber << ".png";// frst sets width=5 den fillsin '0' den blobnumber..all is set in its name...
          //cvSaveImageBlob(filename.str().c_str(), img, it->second); //here it clicks the image from the video streaming in thru camera....
          //blobNumber++;// here it increases the blob number for different images...

          //std::cout << filename.str() << " saved!" << std::endl;  //output the string for the file and then line ends...
        }
        break;
        case 'm':
        while(k!='q')
        {
        fflush(stdin);
        for (CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
        {	blob=it->second;
  		cout << "Blob #" << blob->label << endl;
  		cout << " - Centroid     = (" << blob->centroid.x << ", " << blob->centroid.y << ")" << endl;
  		mouse(blob->centroid.x, blob->centroid.y);
        }
        }
        break;*/
    }
    cout<<"working"<<endl;
    //cvReleaseBlob(blob);
    cvReleaseBlobs(blobs); // blobs structure is released...
    //cvReleaseBlob(blob);
    //cvReleaseBlob(blob1);
    //frameNumber++;
  }

  cvReleaseStructuringElement(&morphKernel); //structuring element is released
  cvReleaseImage(&frame); //den image is also released...
  //cvReleaseBlob(blob);
  
  cvDestroyWindow("red_object_tracking"); //window is destroyed...

  return 0;
}
