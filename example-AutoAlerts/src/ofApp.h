#pragma once

#include "ofMain.h"
#include "ofxModal.h"

class ofApp : public ofBaseApp{

	public:
    
        void setup();
        void keyPressed(int key);
    
        ofxModalConfirm mConfirm;
        shared_ptr<ofxModalAlert> mAlert;
        void onModalEvent(ofxModalEvent e);
		
};
