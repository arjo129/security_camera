#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
time_t lastmotion;
using namespace std;
using namespace cv;

int main(int argc, char** argv){
	cv::Mat frame1,frame2,gray1,gray2,diff, thresh, iblur, finalt,bg;
	bool isBgEnabled=true, confEnabled = false;
	cv::VideoCapture webcam(0);
	cv::VideoWriter writer;
	if(!webcam.isOpened()){
		cout << "Fatal error: could not open webcam \n";
		return -1;
	}

	cv::Mat bim;
	webcam.read(bim);
	cv::cvtColor(bim,bg,CV_RGB2GRAY);
	long i = 0;
	bool recording = false;
	while(1){
		webcam.read(frame1);
		cv::cvtColor(frame1,gray1, CV_RGB2GRAY);
		//cout<<sum(frame1)[0]<<"\n";
		webcam.read(frame2);
		cv::cvtColor(frame2,gray2, CV_RGB2GRAY);
		cv::absdiff(gray1,gray2,diff);
		cv::threshold( diff, thresh, 30, 255,THRESH_BINARY);
		cv::blur(thresh,iblur, Size(10,10), Point(-1,-1));
		cv::threshold( iblur, finalt, 2,255,THRESH_BINARY);
		auto isum = cv::sum(finalt)[0]/(bim.rows*bim.cols);
        time_t start_recording;
		if(isum > 6){
			if(!recording){
				cv::Size csize(webcam.get(CV_CAP_PROP_FRAME_WIDTH),webcam.get(CV_CAP_PROP_FRAME_HEIGHT));
				start_recording = time(NULL);
				char buffer[80];
				strftime (buffer,80,"%I%M%S%p_%a_%F.avi",localtime(&start_recording));
				writer.open(buffer,CV_FOURCC('M','J','P','G'),10,csize);
				recording = true;
				cout << "recording to file "<<buffer<<endl;
			} else {
				writer << frame1;
			}
		}
		else{
			cout << "no motion.. recording stopped"<<endl;
            if(recoding){
                //Give 2 seconds recording grace
                time_t now= time(null)
                double diff_time = difftime(now,start_recording);
                if(diff_time>2.0){
                    recording = false;
                }
                else{
                    writer << frame1;
                }
            }
		}
	}

	return 0;
}
