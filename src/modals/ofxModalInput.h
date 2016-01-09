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
#include "ofxModalWindow.h"

class ofxModalInput : public ofxModalWindow {


    public:
    
        ofxModalInput()
        {
            addButton("ok");
            setTitle("hello");
            setTheme(mTheme);
            addComponents();
        }

        void setTheme(std::shared_ptr<ofxModalTheme> theme)
        {
            ofxModalWindow::setTheme(mTheme);
            ofxDatGuiButton* b1 = getButton("ok");
            b1->setWidth(theme->layout.button.width);
        }
    
        void addComponents()
        {
            theme = new ofxDatGuiThemeMidnight();
            attach(new ofxDatGuiTextInput("input-1", "placeholder"))->setTheme(theme);
            attach(new ofxDatGuiTextInput("input-2", "placeholder"))->setTheme(theme);
            autoSize();
        }


    private:
    
        ofxDatGuiTheme* theme;
    
        void onModalChange(ofxDatGuiButtonEvent e)
        {
            if (e.target == getButton("ok")){
                dispatchCallbacks(ofxModalEvent::CONFIRM);
            }
        }

        void onButtonEvent(ofxDatGuiButtonEvent e)
        {
            hide();
            if (e.target == getButton("ok")){
                dispatchCallbacks(ofxModalEvent::CONFIRM);
            }
        }

};