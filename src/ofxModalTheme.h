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

class ofxModalTheme {

    public:
    
        ofxModalTheme(){
            buttons.close.btnDefault.load("ofxmodal_assets/modal-close.png");
            buttons.close.btnOnHover.load("ofxmodal_assets/modal-close-hover.png");
        
            fonts.title.color = ofColor::fromHex(0x111111);
            fonts.title.load("ofxmodal_assets/HelveticaNeueLTStd-Md.otf", 24);
        
            fonts.message.color = ofColor::fromHex(0x777777);
            fonts.message.load("ofxmodal_assets/HelveticaNeueLTStd-Md.otf", 20);
        }
    
        struct font{
            int size;
            string file;
            ofColor color;
            shared_ptr<ofTrueTypeFont> ttf;
            font(){
                ttf = make_shared<ofTrueTypeFont>();
            }
            void load(string f, int s){
                file = f;
                size = s;
                ttf->load(file, size);
            }
            void draw(string s, int x, int y){
                if (ttf->isLoaded()) ttf->drawString(s, x, y);
            }
            ofRectangle bounds(string s){
                return ttf->getStringBoundingBox(s, 0, 0);
            }
            int width(string s){
                return ttf->getStringBoundingBox(s, 0, 0).width;
            }
            int height(string s){
                return ttf->getStringBoundingBox(s, 0, 0).height;
            }
        };
    
        struct {
            font title;
            font message;
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
            ofColor color = ofColor::fromHex(0x000000);
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