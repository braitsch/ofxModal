#pragma once

#include "ofMain.h"
#include "ofxModal.h"

class ofApp : public ofBaseApp{

	public:
        void setup();
        void update();
        void draw();
    
        shared_ptr<ofxModalAlert> mAlert;
        void onModalEvent(ofxModalEvent e);
		
        ofxDatGuiButton* mShowAlert;
        void onButtonEvent(ofxDatGuiButtonEvent e);
};
