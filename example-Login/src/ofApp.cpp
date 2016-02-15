#include "ofApp.h"

void ofApp::setup()
{
    ofSetWindowPosition(ofGetScreenWidth()/2 - ofGetWidth()/2, 0);
    
// listen for events & show the window //
    mLogin.addListener(this, &ofApp::onLoginEvent);
    mLogin.show();
}

void ofApp::keyPressed(int key)
{
    if (key == 'm'){
        mLogin.show();
    }   else if (key == 'h'){
        mLogin.hide();
    }
}

void ofApp::onLoginEvent(ofxModalEvent e)
{
    if (e.type == ofxModalEvent::CONFIRM){
        cout << "confirm button was selected" << endl;
    }
}

