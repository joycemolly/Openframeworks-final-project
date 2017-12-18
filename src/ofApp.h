#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dCircle {
    
public:
    CustomParticle() {
    }
    ofColor color;
    
    ofImage thePanda;
    void draw() {
        float radius = getRadius();
        thePanda.load("ball.png");
        
        glPushMatrix();
        glTranslatef(getPosition().x-30, getPosition().y-30, 0);

        ofSetColor(color.r, color.g, color.b);
        thePanda.draw(0,0);
//        ofFill();
//        ofCircle(0, 0, radius);
        
        glPopMatrix();
        
        
        
    }
};


// -------------------------------------------------
class testApp : public ofBaseApp {
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void resized(int w, int h);
    
    float                                   px, py;
    bool                                    bDrawLines;
    bool                                    bMouseForce;
    
    ofxBox2d                                box2d;              //    the box2d world
    ofPolyline                              drawing;          //    we draw with this first
    ofxBox2dEdge                            edgeLine;          //    the box2d edge/line shape (min 2 points)
    vector        <shared_ptr<ofxBox2dCircle> >    circles;          //    default box2d circles
    vector        <shared_ptr<ofxBox2dRect> >        boxes;              //    defalut box2d rects
    vector        <shared_ptr<CustomParticle> >    customParticles;  //    this is a custom particle the extends a cirlce
    
    
    
   // --------------------------------
    ofxKinect kinect;
    float scale;
    ofxCvColorImage colorImg;
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
    
    bool bThreshWithOpenCV;
    int nearThreshold;
    int farThreshold;
    
    //ofImage thePanda;
    
    
};
