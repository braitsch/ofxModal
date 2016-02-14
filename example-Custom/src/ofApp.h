#pragma once

#include "ofMain.h"
#include "ModalForm.h"

class ofApp : public ofBaseApp{

	public:
    
        void setup();
        void keyPressed(int key);
    
        ModalForm mForm;
        void onModalEvent(ofxModalEvent e);
		
};
