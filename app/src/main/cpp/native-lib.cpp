#include <jni.h>
#include <string>


#include <thread>

#include <opencv2/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>


#include "timer.h"

#include <iostream>

#include "opencv2/opencv.hpp"
#include <chrono>

using namespace std::chrono;
using namespace cv;
using namespace std;

double startTime, finishTime;


Mat* split_eight(Mat img){
    Mat* eightMat = new Mat[8];

    Mat firste = img(Rect(0, 0, img.cols, img.rows/8));
    Mat seconde = img(Rect(0, img.rows/8, img.cols, img.rows- 7 *img.rows/8));
    Mat thirde = img(Rect(0, img.rows/4, img.cols, img.rows- 7 *img.rows/8));
    Mat fourthe = img(Rect(0, 3*img.rows/8, img.cols, img.rows- 7 *img.rows/8));

    Mat fifthe = img(Rect(0,  img.rows/2,img.cols, img.rows- 7 *img.rows/8));
    Mat sixthe = img(Rect(0, 5*img.rows/8, img.cols, img.rows- 7 *img.rows/8));
    Mat seventhe = img(Rect(0, 6*img.rows/8, img.cols, img.rows- 7 *img.rows/8));
    Mat eighthe = img(Rect(0, 7*img.rows/8, img.cols, img.rows- 7 *img.rows/8));


    eightMat[0]=firste;
    eightMat[1]=seconde;
    eightMat[2]=thirde;
    eightMat[3]=fourthe;

    eightMat[4]=fifthe;
    eightMat[5]=sixthe;
    eightMat[6]=seventhe;
    eightMat[7]=eighthe;
    return eightMat;

}


Mat* split_four(Mat img){
    Mat* fourMat = new Mat[4];

    Mat firstqu = img(Rect(0, 0, img.cols, img.rows/4));
    Mat secondqu = img(Rect(0, img.rows/4, img.cols, img.rows-3*img.rows/4));
    Mat thirdqu = img(Rect(0, img.rows/2, img.cols, img.rows-3*img.rows/4));
    Mat fourthqu = img(Rect(0, 3*img.rows/4, img.cols, img.rows-3*img.rows/4));


    fourMat[0]=firstqu;
    fourMat[1]=secondqu;
    fourMat[2]=thirdqu;
    fourMat[3]=fourthqu;
    return fourMat;

}

Mat* split_two(Mat img){
    Mat* twoMat = new Mat[2];


    Mat firsthalf = img(Rect(0, 0, img.cols, img.rows/2));
//    Mat secondhalf = img(Rect(0, 0, img.cols, img.rows));
    Mat secondhalf = img(Rect(0, img.rows/2, img.cols, img.rows-img.rows/2));
    twoMat[0]=firsthalf;
    twoMat[1]=secondhalf;
    return twoMat;

}

Mat thread_doing(Mat twoimg) {

    Mat sonuc ;
    int MAX_KERNEL_LENGTH = 31;
    cv::Mat out;
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        blur( twoimg, twoimg, Size( i, i ), Point(-1,-1) );
    }
    return twoimg ;
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_example_myproject_MainActivity_MaoJNI(JNIEnv *env, jobject instance,jlong mat ,jint num) {

    Mat &input = *(Mat *) mat;
    int numth = num;

    if (numth == 4) {
        Mat *fourMat = new Mat[4];
        fourMat = split_four(input);

        Mat firsth, secondh;

        std::thread first(thread_doing, fourMat[0]);
        std::thread second(thread_doing, fourMat[1]);

        std::thread third(thread_doing, fourMat[2]);
        std::thread fourth(thread_doing, fourMat[3]);


        first.join();                // pauses until first finishes
        second.join();

        third.join();                // pauses until first finishes
        fourth.join();

        vconcat(fourMat[0], fourMat[1], firsth);
        vconcat(fourMat[2], fourMat[3], secondh);
        vconcat(firsth, secondh, input);

    }

    else if (numth == 2) {
        Mat *twoMat = new Mat[2];
        twoMat = split_two(input);

        std::thread first(thread_doing, twoMat[0]);
        std::thread second(thread_doing, twoMat[1]);

        first.join();                // pauses until first finishes
        second.join();


        vconcat(twoMat[0], twoMat[1], input);

    }
    else if (numth ==1){

        int MAX_KERNEL_LENGTH = 31;
        for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
        {
            blur( input, input, Size( i, i ), Point(-1,-1) );
        }    // pauses until first finishes


    }
    else if (numth == 8) {
        Mat *eightMat = new Mat[8];
        eightMat = split_eight(input);

        Mat firsth, secondh, thirdh, forthh;

        std::thread first(thread_doing, eightMat[0]);
        std::thread second(thread_doing, eightMat[1]);

        std::thread third(thread_doing, eightMat[2]);
        std::thread fourth(thread_doing, eightMat[3]);

        std::thread fifth(thread_doing, eightMat[4]);
        std::thread sixth(thread_doing, eightMat[5]);

        std::thread seventh(thread_doing, eightMat[6]);
        std::thread eighth(thread_doing, eightMat[7]);


        first.join();                // pauses until first finishes
        second.join();

        third.join();                // pauses until first finishes
        fourth.join();

        fifth.join();                // pauses until first finishes
        sixth.join();

        seventh.join();                // pauses until first finishes
        eighth.join();

        Mat firstha, secondha;

        vconcat(eightMat[0], eightMat[1], firsth);
        vconcat(eightMat[2], eightMat[3], secondh);

        vconcat(firsth, secondh, firstha);

        vconcat(eightMat[4], eightMat[5], thirdh);
        vconcat(eightMat[6], eightMat[7], forthh);

        vconcat(thirdh, forthh, secondha);

        vconcat(firstha, secondha, input);

    }

    GET_TIME(finishTime);
    double totalTime = finishTime - startTime;
    float t = 0.35656;

    return totalTime;
}