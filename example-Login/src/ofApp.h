#pragma once

#include "ofMain.h"
#include "ModalLogin.h"

class ofApp : public ofBaseApp{

	public:
    
        void setup();
        void keyPressed(int key);
    
        ModalLogin mLogin;
        shared_ptr<ofxModalAlert> mAlert;
        void onLoginEvent(ofxModalEvent e);
		
};
