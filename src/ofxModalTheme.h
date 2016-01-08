/*
    Copyright (C) 2016 Stephen Braitsch [http://braitsch.io]

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once
#include "ofImage.h"
#include "ofColor.h"
#include "ofGraphics.h"
#include "ofTrueTypeFont.h"
#include "ofxSmartFont.h"

inline static ofColor hex(int n) { return ofColor::fromHex(n); }

class ofxModalTheme {

    public:
    
        ofxModalTheme(){
            buttons.close.btnDefault.load("ofxbraitsch/ofxmodal/modal-close.png");
            buttons.close.btnOnHover.load("ofxbraitsch/ofxmodal/modal-close-hover.png");
            fonts.title = ofxSmartFont::add("ofxbraitsch/fonts/HelveticaNeueLTStd-Md.otf", 24);
            fonts.message = ofxSmartFont::add("ofxbraitsch/fonts/Verdana.ttf", 20);
        }
    
    /*
        default color palette
    */
    
        struct{
            struct{
                struct{
                    ofColor label = hex(0x1A1A1A);
                    ofColor labelOnMouseOver = hex(0x777777);
                    ofColor labelOnMouseDown = hex(0x222222);
                    ofColor background = hex(0x1A1A1A);
                    ofColor backgroundOnMouseOver = hex(0x777777);
                    ofColor backgroundOnMouseDown = hex(0x222222);
                    ofColor border = hex(0x1A1A1A);
                    ofColor borderOnMouseOver = hex(0x777777);
                    ofColor borderOnMouseDown = hex(0x222222);
                } confirm;
            } button;
            struct{
                ofColor title = hex(0x111111);
                ofColor body = hex(0x777777);
            } text;
            struct{
                ofColor header = ofColor::whiteSmoke;
                ofColor body = ofColor::whiteSmoke;
                ofColor footer = ofColor::whiteSmoke;
                ofColor divider = ofColor::black;
            } modal;
            struct{
                ofColor background = ofColor::black;
            } window;
        } color;
    
        struct{
            struct{
                int width = 140;
                int height = 80;
            } button;
            struct{
                float spacing = 14.0f;
            } text;
            struct{
                float opacity = 1.0f;
            } modal;
            struct{
                float opacity = 0.7f;
            } window;
        } layout;
    
    /*
        layout, sizing and rendering rules
    */
    
        struct {
            shared_ptr<ofxSmartFont> title;
            shared_ptr<ofxSmartFont> message;
        } fonts;
    
        struct {
            int width = 800;
            int height = 600;
            int padding = 30;
            ofColor color = ofColor::whiteSmoke;
        } modal;
    
        struct {
            float speed = 0.4f;
        } animation;
    
        struct {
            float opacity = 0.7f;
            ofColor color = hex(0x000000);
        } background;
    
        struct {
            struct {
                int width = 26;
                int height = 26;
                int hitPadding = 20;
                ofImage btnDefault;
                ofImage btnOnHover;
            } close;
        } buttons;


};

