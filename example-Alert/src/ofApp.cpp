#include "ofApp.h"

void ofApp::setup()
{
    ofSetWindowShape(1920, 1080);
    ofSetWindowPosition(ofGetScreenWidth()/2 - ofGetWidth()/2, 0);

    mAlert = make_shared<ofxModalAlert>();
    mAlert->addListener(this, &ofApp::onModalEvent);

    mShowAlert = new ofxDatGuiButton("show alert");
    mShowAlert->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    mShowAlert->setPosition(ofGetWidth()/2 - mShowAlert->getWidth()/2, 450);
    mShowAlert->onButtonEvent(this, &ofApp::onButtonEvent);
    
}

void ofApp::update()
{
    mShowAlert->update();
}

void ofApp::draw()
{
    mShowAlert->draw();
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target == mShowAlert) mAlert->alert("hello!");
}

void ofApp::onModalEvent(ofxModalEvent e)
{
    if (e.type == ofxModalEvent::CANCEL){
        cout << "cancel button was selected" << endl;
    }   else if (e.type == ofxModalEvent::CONFIRM){
        cout << "confirm button was selected" << endl;
    }
}

