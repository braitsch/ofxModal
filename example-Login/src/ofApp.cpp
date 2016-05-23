#include "ofApp.h"

void ofApp::setup()
{
    ofSetWindowPosition(ofGetScreenWidth()/2 - ofGetWidth()/2, 0);

// create a global application alert //
    mAlert = make_shared<ofxModalAlert>();
    
// listen for events & show the window //
    mLogin.addListener(this, &ofApp::onLoginEvent);
    mLogin.setAlert(mAlert);
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
    if (e.type == ofxModalEvent::CANCEL){
        cout << "cancel button was selected" << endl;
    }   else if (e.type == ofxModalEvent::CONFIRM){
    // check for valid login data here //
        cout << "confirm button was selected" << endl;
    // show an alert if login was successful //
        mLogin.alert("LOGIN SUCCESSFUL!");
    }
}

