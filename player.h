#ifndef PLAYER_H
#define PLAYER_H
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <QFile>
#include <QDir>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;

class Player : public QThread
{    Q_OBJECT
 private:
     bool stoped;
     QMutex mutex;
     QWaitCondition condition;
     Mat frame;
     Mat captureFrame;
     Mat detectionFrame;
     int frameRate;
     VideoCapture capture;
     QImage captureImage;
     QImage detectionImage;
     CascadeClassifier faceCascade;
     CascadeClassifier eyeCascade;
     //las cargo como resource
     char *faceCascadeXML = ":/cascades/cascades/haarcascade_frontalface_alt.xml";
     char *eyeCascadeXML = ":/cascades/cascades/haarcascade_mcs_eyepair_big.xml";
 signals:
      //Signal to output the captured frame
      void sigCaptureImage(const QImage &image);
      //Signal to output the ROI
      void sigDetectionImage(const QImage &image);
      //Signal to output capture result
      void sigDetectionResult(const char *message);
 protected:
     void run();
     void msleep(int ms);
     QImage mat2Qimage(Mat frame);
 public:
    //Constructor
    Player(QObject *parent = 0);
    //Destructor
    ~Player();
    //Load a video from memory or open a cammera
    bool loadVideo();
    //Play the video
    void play();
    //Stop the video
    void stop();
    //Check if the player has been stopped
    bool isStopped() const;
    //Detect face in capture images
    void detectFace(Mat frame);
    //Detect eyes in capture images
    void detectEyes(Mat frame);
};
#endif // VIDEOPLAYER_H
