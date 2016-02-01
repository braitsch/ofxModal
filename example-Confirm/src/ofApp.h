#pragma once

#include "ofMain.h"
#include "ofxModal.h"

class ofApp : public ofBaseApp{

	public:
        void setup();
        void update();
        void draw();
        void keyPressed(int key);
    
        ofxModalConfirm confirm;
        void onModalEvent(ofxModalEvent e);
		
};
