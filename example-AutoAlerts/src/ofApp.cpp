#include "ofApp.h"

void ofApp::setup()
{
    ofSetWindowPosition(ofGetScreenWidth()/2 - ofGetWidth()/2, 0);
    
// add a global alert //
    mAlert = make_shared<ofxModalAlert>();
    mAlert->setMessageAlignment(ofxParagraph::ALIGN_CENTER);

// setup a simple confirm window //
    mConfirm.addListener(this, &ofApp::onModalEvent);
    mConfirm.setMessage("Are you sure you really want to do this?");
    mConfirm.setMessageAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
// add assign it our global alert //
    mConfirm.setAlert(mAlert);
    mConfirm.show();
}

void ofApp::keyPressed(int key)
{
    if (key == 'm'){
        mConfirm.show();
    }   else if (key == 'h'){
        mConfirm.hide();
    }
}

void ofApp::onModalEvent(ofxModalEvent e)
{
    if (e.type == ofxModalEvent::CONFIRM){
        mConfirm.alert("**************** Congratulations! You Won! ****************");
    }
}

