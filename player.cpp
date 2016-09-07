#include "player.h"

Player::Player(QObject *parent): QThread(parent)
{
    stoped = true;

    QString sPath = QDir::currentPath().append("/frontalface.xml");
    QFile::copy(faceCascadeXML , sPath);
    faceCascade.load(sPath.toStdString());
    QFile::remove(sPath);
    if (faceCascade.empty()){
        //#TODO mejorar. el envio de una señal no es efectivo dado que la ventana no esta creada
        emit sigDetectionResult("Error al cargar Haar Cascade de rostro");

    }
    sPath = QDir::currentPath().append("/eyes.xml");
    QFile::copy(eyeCascadeXML , sPath);
    eyeCascade.load(sPath.toStdString());
    QFile::remove(sPath);
    if (eyeCascade.empty()){
        emit sigDetectionResult("Error al cargar Haar Cascade de ojos");
    }
}


bool Player::loadVideo() {
    capture.open(0);
    if (capture.isOpened())
    {
        //frameRate = (int) capture.get(CV_CAP_PROP_FPS);
        frameRate = 25;
        return true;
    }
    else
        return false;
}

void Player::play()
{
    if (!isRunning()) {
        if (isStopped()){
            stoped = false;
        }
        loadVideo();
        start(LowPriority);
    }
}

QImage Player::mat2Qimage(Mat frame){
    QImage image;
    if (frame.channels()== 3){
        cv::cvtColor(frame, frame, CV_BGR2RGB);
        image = QImage((const unsigned char*)(frame.data),
                          frame.cols,frame.rows,QImage::Format_RGB888);
    }
    else
    {
        image = QImage((const unsigned char*)(frame.data),
                             frame.cols,frame.rows,QImage::Format_Indexed8);
    }
    return image;
}

void Player::run()
{
    //int delay = (1000/frameRate);
    int delay = 40;
    while(!stoped){
        if (!capture.read(frame))
        {
            stoped = true;
        }
        captureImage = mat2Qimage(frame);
        emit sigCaptureImage(captureImage);
        detectFace(frame);
        this->msleep(delay);
    }
}

void Player::detectFace(Mat frame){
    if (!faceCascade.empty()){
        Mat grayFrame;
        cvtColor(frame, grayFrame, CV_BGR2GRAY);
        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(grayFrame, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
        if (faces.size()>0){
            if (faces.size()>1){
                emit sigDetectionResult("Se ha detectado más de una cara en la imagen");
            }
            else {
                Point center( faces[0].x + faces[0].width*0.5, faces[0].y + faces[0].height*0.5 );
                ellipse( grayFrame, center, Size( faces[0].width*0.5, faces[0].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
                detectionImage = mat2Qimage(grayFrame);
                emit sigDetectionImage(detectionImage);
                emit sigDetectionResult("Se ha detectado una cara en la imagen");
                detectEyes(grayFrame);
            }
        }
    }
    else{
        emit sigDetectionResult("Error al cargar Haar Cascade de rostro");
    }
}

void Player::detectEyes(Mat grayFrame){
    if (!eyeCascade.empty()){

        std::vector<cv::Rect> eyes;
        eyeCascade.detectMultiScale(grayFrame, eyes, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
        if (eyes.size()>0){
            Point center( eyes[0].x + eyes[0].width*0.5, eyes[0].y + eyes[0].height*0.5 );
            ellipse( grayFrame, center, Size( eyes[0].width*0.5, eyes[0].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
            detectionImage = mat2Qimage(grayFrame);
            emit sigDetectionImage(detectionImage);
            emit sigDetectionResult("Se ha detectado un par de ojos en la imagen");
        }
    }
}


Player::~Player()
{
    mutex.lock();
    stoped = true;
    capture.release();
    condition.wakeOne();
    mutex.unlock();
    wait();
}

void Player::stop()
{
    stoped = true;
    capture.release();
}

void Player::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

bool Player::isStopped() const{
    return this->stoped;
}
