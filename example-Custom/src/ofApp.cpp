#include "ofApp.h"

void ofApp::setup()
{
    ofSetWindowPosition(ofGetScreenWidth()/2 - ofGetWidth()/2, 0);
    
// listen for events //
    mForm.addListener(this, &ofApp::onModalEvent);
    
// set a message and and show the modal //
    mForm.setMessage("press 'h' to hide this window, press 'm' to show it.");
    mForm.show();
}

void ofApp::keyPressed(int key)
{
    if (key == 'm'){
        mForm.show();
    }   else if (key == 'h'){
        mForm.hide();
    }
}

void ofApp::onModalEvent(ofxModalEvent e)
{
    if (e.type == ofxModalEvent::SHOWN){
        cout << "modal window is open" << endl;
    }   else if (e.type == ofxModalEvent::HIDDEN){
        cout << "modal window is closed" << endl;
    }   else if (e.type == ofxModalEvent::CANCEL){
        cout << "cancel button was selected" << endl;
    }   else if (e.type == ofxModalEvent::CONFIRM){
        cout << "confirm button was selected" << endl;
    }
}

