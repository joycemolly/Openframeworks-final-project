#include "ofApp.h"
static int pts[] = {257,219,257,258,259,274,263,325,266,345,266,352,269,369,276,387,286,415,291,425,302,451,308,462,316,472,321,480,328,488,333,495,339,501,345,505,350,507,365,515,370,519,377,522,382,525,388,527,405,534,426,538,439,539,452,539,468,540,485,540,496,541,607,541,618,539,625,537,641,530,666,513,682,500,710,476,723,463,727,457,729,453,732,450,734,447,738,440,746,423,756,404,772,363,779,343,781,339,784,327,789,301,792,278,794,267,794,257,795,250,795,232,796,222,796,197,797,195,797,188,796,188};
static int nPts  = 61*2;

//--------------------------------------------------------------
void testApp::setup() {
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    bMouseForce = false;
    
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.createGround();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    //thePanda.load("pan.png");
    
    // lets add a contour to start
//    for (int i=0; i<nPts; i+=2) {
//        float x = pts[i];
//        float y = pts[i+1];
//        edgeLine.addVertex(x, y);
//    }
//
    // make the shape
    edgeLine.setPhysics(0.0, 0.5, 0.5);
    edgeLine.create(box2d.getWorld());
  //  -----------------------------------------
    ofSetLogLevel(OF_LOG_VERBOSE);
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    nearThreshold = 230;
    farThreshold = 70;
    bThreshWithOpenCV = true;
    
    ofSetFrameRate(60);
    
    scale=1.8;
    
}

//--------------------------------------------------------------
void testApp::update() {
    
    std::cout<<nearThreshold<<endl;
    std::cout<<farThreshold<<endl;
    
//
    box2d.update();
    
    
  
    
    if(bMouseForce) {
        float strength = 8.0f;
        float damping  = 0.7f;
        float minDis   = 100;
        for(int i=0; i<circles.size(); i++) {
            circles[i].get()->addAttractionPoint(mouseX, mouseY, strength);
            circles[i].get()->setDamping(damping, damping);
        }
        for(int i=0; i<customParticles.size(); i++) {
            customParticles[i].get()->addAttractionPoint(mouseX, mouseY, strength);
            customParticles[i].get()->setDamping(damping, damping);
        }
        
    }
    
    // remove shapes offscreen
    ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(customParticles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
    //---------------------------------------
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        grayImage.setFromPixels(kinect.getDepthPixels());
        
        if(bThreshWithOpenCV) {
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        } else {
            ofPixels & pix = grayImage.getPixels();
            int numPixels = pix.size();
            for(int i = 0; i < numPixels; i++) {
                if(pix[i] < nearThreshold && pix[i] > farThreshold) {
                    pix[i] = 255;
                } else {
                    pix[i] = 0;
                }
            }
            
            
            
        }
        
        grayImage.flagImageChanged();
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
        
    }
    
    
    //----------------------
    for(int i=contourFinder.blobs.size()-1; i>=0; i--)
    {
        
        vector<ofPoint> pts = contourFinder.blobs[i].pts;
        
        drawing.clear();

//        drawing.addVertices(pts);
        
//        ofxBox2dPolygon poly;
        
        for(int i = 0; i<pts.size(); i++)
        {
//            poly.addVertex(pts[i]);
            
            ofPoint p= pts[i];
            p*=scale;
            drawing.addVertex(p);
            
            

        }
//        poly.setAsEdge(false);
//        poly.triangulate(15);
//        poly.setPhysics(1.0, 0.3, 0.3);
//        poly.create(box2d.getWorld());
//        boxes.push_back(poly);
    }
    
    if((int)ofRandom(0, 10) == 0) {
        ofxBox2dCircle c;
        c.setPhysics(0.3, 0.5, 0.1);
        c.setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-20, 20), -20, ofRandom(10, 20));
//        circles.push_back(c);
    }
    
    
    
    
    
}


//--------------------------------------------------------------
void testApp::draw() {
    
    
    ofSetColor(0);
    
    
    // draw from the live kinect
    // kinect.drawDepth(0,0,1024, 768);
    //kinect.draw(0, 0, 1024, 768);
    
    //grayImage.draw(0,0,1024,768);
    
    //contourFinder.draw(0,0,1024,768);
    

    for(int i=0; i<circles.size(); i++) {
        ofFill();
        ofSetHexColor(0x90d4e3);
        circles[i].get()->draw();
    }
    
    for(int i=0; i<boxes.size(); i++) {
        ofFill();
        ofSetHexColor(0xe63b8b);
        boxes[i].get()->draw();
    }
    
    for(int i=0; i<customParticles.size(); i++) {
        customParticles[i].get()->draw();
    }
    
    ofNoFill();
    ofSetHexColor(0x444342);
    if(drawing.size()==0) {
        edgeLine.updateShape();
        edgeLine.draw();
    }
    else drawing.draw();
    
    
    string info = "";
    info += "Press [s] to draw a line strip ["+ofToString(bDrawLines)+"]\n";
    info += "Press [f] to toggle Mouse Force ["+ofToString(bMouseForce)+"]\n";
    info += "Press [c] for circles\n";
    info += "Press [b] for blocks\n";
    info += "Press [z] for custom particle\n";
    info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
    info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
    info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
    info += "farThreshold "+ofToString(farThreshold)+"\n";
    info += "nearThreshold "+ofToString(nearThreshold)+"\n";
    info += "scale "+ofToString(scale)+"\n";

    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);



    

//    ----------------extra shapes-----------
    
    //    ofSetHexColor(0x444342);
//    ofFill();
//    for (int i=0; boxes.size(); i++) {
//        boxes[i]->draw();
//    }
//
//    // some circles :)
//    for (int i=0; i<circles.size(); i++) {
//        ofFill();
//        ofSetHexColor(0xc0dd3b);
//        circles[i]->draw();
//    }
    


}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    if(key == 'c') {
        float r = ofRandom(4, 10);        // a random radius 4px - 20px
        circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(3.0, 0.53, 0.1);
        circles.back().get()->setup(box2d.getWorld(), mouseX, mouseY, r);
        
    }
    
    if(key == 'b') {
        float w = ofRandom(4, 20);
        float h = ofRandom(4, 20);
        boxes.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
        boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
        boxes.back().get()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
    }
    
    if(key == 'z') {
        
        customParticles.push_back(shared_ptr<CustomParticle>(new CustomParticle));
        CustomParticle * p = customParticles.back().get();
//        float r = ofRandom(3, 10);        // a random radius 4px - 20px
        float r = 20;
        p->setPhysics(0.4, 0.53, 0.31);
        p->setup(box2d.getWorld(), mouseX, mouseY, r);
//        p->color.r = ofRandom(20, 100);
//        p->color.g = 0;
//        p->color.b = ofRandom(150, 255);
        p->color.r = 255;
        p->color.g = 255;
        p->color.b = 255;

    }
    
    if(key == 'f') bMouseForce = !bMouseForce;
    if(key == 't') ofToggleFullscreen();
    
    
    switch (key) {
        case ' ':
            bThreshWithOpenCV = !bThreshWithOpenCV;
            break;
            
            
            
        case '>':
        case '.':
            farThreshold ++;
            if (farThreshold > 255) farThreshold = 255;
            break;
            
        case '<':
        case ',':
            farThreshold --;
            if (farThreshold < 0) farThreshold = 0;
            break;
            
        case '+':
        case '=':
            nearThreshold ++;
            if (nearThreshold > 255) nearThreshold = 255;
            break;
            
        case '-':
            nearThreshold --;
            if (nearThreshold < 0) nearThreshold = 0;
            break;
            
        case 'p':
            scale+=0.1;
           
            break;
            
        case 'o':
            scale-=0.1;
            
            break;
     
            
            
            
        
            
            
            
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
    drawing.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    
    if(edgeLine.isBody()) {
        drawing.clear();
        edgeLine.destroy();
    }
    
    drawing.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
    
    drawing.setClosed(false);
    drawing.simplify();
    
    edgeLine.addVertexes(drawing);
    //polyLine.simplifyToMaxVerts(); // this is based on the max box2d verts
    edgeLine.setPhysics(0.0, 0.5, 0.5);
    edgeLine.create(box2d.getWorld());
    
    drawing.clear();
    
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
    
}

