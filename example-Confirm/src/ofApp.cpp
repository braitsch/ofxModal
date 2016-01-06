#include "ofApp.h"

void ofApp::setup()
{
    ofSetWindowPosition(ofGetScreenWidth()/2 - ofGetWidth()/2, 0);
    confirm.onModalEvent(ofxModalEvent::CANCEL, this, &ofApp::onModalCancel);
    confirm.onModalEvent(ofxModalEvent::CONFIRM, this, &ofApp::onModalConfirm);
}

void ofApp::update() { }
void ofApp::draw() { }

void ofApp::keyPressed(int key)
{
    if (key == 'm'){
        confirm.show();
    }   else if (key == 'h'){
        confirm.hide();
    }
}

void ofApp::onModalCancel(ofxModalEvent e)
{
    cout << "cancel button was selected" << endl;
}

void ofApp::onModalConfirm(ofxModalEvent e)
{
    cout << "confirm button was selected" << endl;
}

