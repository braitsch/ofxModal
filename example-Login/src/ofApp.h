#pragma once

#include "ofMain.h"
#include "ModalLogin.h"

class ofApp : public ofBaseApp{

	public:
    
        void setup();
        void keyPressed(int key);
    
        ModalLogin mLogin;
        void onLoginEvent(ofxModalEvent e);
		
};
