#ifndef OSSIAVID_H
#define OSSIAVID_H

#include "ofMain.h"

#define MATRIX_SIZE 32

#ifdef KINECT
#include "ofxKinect.h"
#endif

#ifdef CV
#include "ofxOpenCv.h"
#endif

//--------------------------------------------------------------
class ossiaVid
{
public:
    ossiaVid();
    ofParameterGroup params;

    void checkResize();

    struct canvas
    {
    void corner2center(const unsigned int* wAndH, const float& reSize, const ofVec3f& center);
    float x, y, z;
    unsigned int h, w;
    };

protected:
    float prevSize;
    ofVec3f prevPlace;
    ofParameter<ofVec3f> placement;
    ofParameter<float> size;
    unsigned int vidWandH[2];
    canvas canv;

    ofParameter<ofVec4f> color;

    ofParameter<bool> drawVid;

    ofParameterGroup pixControl;
    ofParameter<bool> getPixels;
    ofParameter<bool> invert;
    ofParameter<int> hPoints;
    ofParameter<int> vPoints;
    ofParameter<float> threshold;
    ofParameter<float> pixVal[MATRIX_SIZE * MATRIX_SIZE];
    ofParameter<ofVec4f> averageColor;
    ofParameter<ofVec3f> centroid;
    ofParameter<bool> drawCircles;
    ofParameter<bool> drawCenter;
    ofParameter<float> circleSize;
    ofParameter<int> circleResolution;
    ofParameter<ofVec4f> circleColor;
    ofParameterGroup pixMatrix;

    void setMatrix(ofParameterGroup& params);
    void processPix(const ofPixels& px, ofParameter<float>* pv, const canvas& cnv);
    void drawPix(ofParameter<float>* pv, const canvas& cnv);
};

//--------------------------------------------------------------
#ifdef CV
class ossiaCv
{
protected:
    ofParameterGroup cvControl;

    ofParameter<int> maxThreshold;

    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage; // grayscale depth image

    ofxCvGrayscaleImage grayBg;
    ofParameter<bool> holdBackGround;
    void setBackGround(bool& hold);

    ofParameter<bool> drawCvImage;

    ofParameterGroup blobs;
    ofParameter<int> minArea;
    ofParameter<int> maxArea;
    // blob position and size
    ofParameter<ofVec3f> position[10];
    ofParameter<int> area[10];

    void cvSetup(const unsigned int* wAndH, ofParameterGroup& group);
    void cvUpdate(ofPixels& pixels);
    void cvdraw(const ossiaVid::canvas& cnv);

    ofxCvContourFinder contourFinder;
    ofParameter<bool> drawContours;
};
#endif

//--------------------------------------------------------------
class ossiaPlayer: public ossiaVid
        #ifdef CV
        , protected ossiaCv
        #endif
{
public:
    ossiaPlayer(string path);
    void setup();
    void update();
    void draw();
    void close();
    ofVideoPlayer vid;

private:
    string path;

    ofParameter<bool> play;
    void setPlay(bool &toPlay);

    ofParameter<bool> loop;
    void setLoop(bool &toLoop);

    ofParameter<float> seek;
    void setFrame(float &toSeek);

    ofParameter<float> volume;
    void setVolume(float &toAmp);
};

//--------------------------------------------------------------
class ossiaGrabber: public ossiaVid
        #ifdef CV
        , protected ossiaCv
        #endif
{
public:
    ossiaGrabber(ofVideoDevice dev);
    void setup(unsigned int width, unsigned int height);
    void update();
    void draw();
    void close();
    ofVideoGrabber vid;

private:
    ofVideoDevice device;

    ofParameter<bool> freeze;
};

//--------------------------------------------------------------
#ifdef KINECT
class ossiaKinect: public ossiaVid
        #ifdef CV
        , protected ossiaCv
        #endif
{
public:
    ossiaKinect(int dev);
    void setup(bool infrared);
    void update();
    void draw();
    void close();
    ofxKinect vid;

private:    
    int device;

    ofParameter<bool> freeze;
};
#endif

#endif // OSSIAVID_H
