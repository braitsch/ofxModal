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
#include "ofAppRunner.h"
#include "ofxDatGui.h"
#include "ofxParagraph.h"
#include "ofxModalEvent.h"
#include "ofxModalTheme.h"

class ofxModal {
    
    public:

        void show()
        {
        // ensure we never show two at the same time //
            if (activeModal == nullptr){
                mVisible = true;
                activeModal = this;
                mState = FADING_IN;
                mAnimation.nTicks = 0;
                mAnimation.percent = 0;
                ofAddListener(ofEvents().draw, this, &ofxModal::onDraw);
                ofAddListener(ofEvents().update, this, &ofxModal::onUpdate);
                ofAddListener(ofEvents().mouseMoved, this, &ofxModal::onMouseMove);
                ofAddListener(ofEvents().mousePressed, this, &ofxModal::onMousePress);
                ofAddListener(ofEvents().windowResized, this, &ofxModal::onWindowResize);
            }
        }
    
        void hide()
        {
            mVisible = true;
            mState = FADING_OUT;
            mAnimation.nTicks = 0;
            mAnimation.percent = 0;
        }
    
        void setSize(int w, int h)
        {
            mModal.width = w;
            mModal.height = h;
            mModal.x = ofGetWidth() / 2 - mModal.width / 2;
            mMessage.text.setWidth(mModal.width - (mModal.padding * 2));
        }
    
        void setTitle(string text)
        {
            mTitle.text = text;
            mTitle.height = mTitle.font->height(mTitle.text);
        }
    
        void setMessage(string text)
        {
            mMessage.visible = true;
            mMessage.text.setText(text);
            mMessage.height = mMessage.text.getStringHeight();
        }
    
        void setTheme(std::shared_ptr<ofxModalTheme> theme, bool applyToAll = false)
        {
            if (applyToAll){
                for(int i=0; i<modals.size(); i++){
                    modals[i]->setTheme(theme, false);
                }
            }   else{
                mModal.color = theme->modal.color;
                mModal.padding = theme->modal.padding;
                setSize(theme->modal.width, theme->modal.height);
                mCloseButton.rect.width = theme->buttons.close.width;
                mCloseButton.rect.height = theme->buttons.close.height;
                mCloseButton.hitPadding = theme->buttons.close.hitPadding;
                mCloseButton.btnDefault = &theme->buttons.close.btnDefault;
                mCloseButton.btnOnHover = &theme->buttons.close.btnOnHover;
                mAnimation.tTicks = theme->animation.speed * ofGetFrameRate();
                mAnimation.tOpacity = theme->background.opacity;
                mTitle.font = &theme->fonts.title;
                mTitle.color = theme->fonts.title.color;
                mMessage.text.setFont(theme->fonts.message.ttf);
                mMessage.text.setColor(theme->fonts.message.color);
                mHeaderHeight = mModal.padding * 2 + mCloseButton.rect.height;
                mFooterHeight = mModal.padding * 3;
            }
        }
    
        static bool visible();
        int getWidth() { return mModal.width; }
        int getHeight() { return mModal.height; }
        int getPadding() { return mModal.padding; }
    
        template<typename T, typename args, class ListenerClass>
        void onModalEvent(ModalEvent::EventType event, T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            subscribers.push_back({event, std::bind(listenerMethod, owner, _1)});
        }

    
    protected:
    
        ofxModal();
        virtual void draw() = 0;
        virtual void update() = 0;
    
        void attachFooterButton(ofxDatGuiButton* btn)
        {
            mFooterButtons.push_back(btn);
        }
    
        void dispatchCallbacks(ModalEvent::EventType eType)
        {
            for(auto s: subscribers){
                if (s.eType == eType){
                    s.callback(ModalEvent(eType, this));
                }
            }
        }

    private:
    
        void onDraw(ofEventArgs &e)
        {
            ofPushStyle();
        // draw background blackout //
            ofSetColor(0, 0, 0, mAnimation.nOpacity);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        // draw modal background //
            ofSetColor(mModal.color);
            ofDrawRectangle(mModal.x, mModal.y, mModal.width, mModal.height);
        // draw title //
            ofSetColor(mTitle.color);
            mTitle.font->draw(mTitle.text, mTitle.x, mTitle.y);
            ofDrawLine(mLine1.p1, mLine1.p2);
            ofDrawLine(mLine2.p1, mLine2.p2);
        // draw message //
            if (mMessage.visible) mMessage.text.draw();
        // draw close button //
            ofSetColor(ofColor::white);
            if (mCloseButton.mouseOver == false){
                mCloseButton.btnDefault->draw(mCloseButton.rect);
            }   else{
                mCloseButton.btnOnHover->draw(mCloseButton.rect);
            }
            ofPopStyle();
        // derived classes draw //
            draw();
        }
    
        void onUpdate(ofEventArgs &e)
        {
            if (mState == FADING_IN || mState == FADING_OUT){
                animate();
            }   else{
                update();
            }
            layout();
        }
    
        inline void layout()
        {
            mTitle.x = mModal.x + mModal.padding;
            mTitle.y = mModal.y + mModal.padding + mCloseButton.rect.height/2 + mTitle.height/2;
            mLine1.p1.x = mModal.x;
            mLine1.p1.y = mModal.y + mHeaderHeight;
            mLine1.p2.x = mModal.x + mModal.width;
            mLine1.p2.y = mLine1.p1.y;
            mLine2.p1.x = mLine1.p1.x;
            mLine2.p1.y = mModal.y + mModal.height - mFooterHeight;
            mLine2.p2.x = mLine1.p2.x;
            mLine2.p2.y = mLine2.p1.y;
            mMessage.text.setPosition(mLine1.p1.x + mModal.padding, mLine1.p1.y + mModal.padding + mMessage.height);
            mCloseButton.rect.x = mModal.x + mModal.width - mModal.padding - mCloseButton.rect.width;
            mCloseButton.rect.y = mModal.y + mModal.padding;
            mCloseButton.hitRect.x = mCloseButton.rect.x - mCloseButton.hitPadding;
            mCloseButton.hitRect.y = mCloseButton.rect.y - mCloseButton.hitPadding;
            mCloseButton.hitRect.width = mCloseButton.rect.width + (mCloseButton.hitPadding * 2);
            mCloseButton.hitRect.height = mCloseButton.rect.height + (mCloseButton.hitPadding * 2);
            for(int i=0; i<mFooterButtons.size(); i++){
                int buttonSpacing = 4;
                int w = mFooterButtons[0]->getWidth();
                int x = mModal.x + mModal.width - mModal.padding - w;
                int y = mLine2.p1.y + mFooterHeight/2 - mFooterButtons[i]->getHeight()/2;
                x -= (w+buttonSpacing) * i;
                mFooterButtons[i]->setPosition(x, y);
            }
        }
    
        void animate()
        {
            mAnimation.nTicks++;
            if (mState == FADING_IN){
                mAnimation.percent = easeInOutQuad(float(mAnimation.nTicks)/mAnimation.tTicks);
            }   else if (mState == FADING_OUT) {
                mAnimation.percent = 1.0f - easeInOutQuad(float(mAnimation.nTicks)/mAnimation.tTicks);
            }
            mAnimation.nOpacity = mAnimation.percent * (mAnimation.tOpacity * 255);
            mModal.y = -mModal.height + mAnimation.percent * (ofGetHeight()/2 - mModal.height/2 + mModal.height);
            if (mAnimation.nTicks == mAnimation.tTicks){
                if (mState == FADING_IN){
                    mState = VISIBLE;
                    dispatchCallbacks(ModalEvent::SHOWN);
                }   else if (mState == FADING_OUT){
                    mState = HIDDEN;
                // modal is closed, ok to show another one now //
                    activeModal = nullptr;
                    dispatchCallbacks(ModalEvent::HIDDEN);
                    ofRemoveListener(ofEvents().draw, this, &ofxModal::onDraw);
                    ofRemoveListener(ofEvents().update, this, &ofxModal::onUpdate);
                    ofRemoveListener(ofEvents().mouseMoved, this, &ofxModal::onMouseMove);
                    ofRemoveListener(ofEvents().mousePressed, this, &ofxModal::onMousePress);
                }
            }
        }
    
        void onMousePress(ofMouseEventArgs &e)
        {
            ofPoint mouse = ofPoint(e.x, e.y);
            if (ofRectangle(mModal.x, mModal.y, mModal.width, mModal.height).inside(mouse) == false) {
                hide();
            }   else if (mCloseButton.hitRect.inside(mouse)){
                hide();
            }
        }
    
        void onMouseMove(ofMouseEventArgs &e)
        {
            ofPoint mouse = ofPoint(e.x, e.y);
            mCloseButton.mouseOver = mCloseButton.hitRect.inside(mouse);
        }
    
        void onWindowResize(ofResizeEventArgs &e)
        {
            mModal.x = ofGetWidth() / 2 - mModal.width / 2;
            mModal.y = -mModal.height + mAnimation.percent * (ofGetHeight()/2 - mModal.height/2 + mModal.height);
        }
    
        double easeInOutQuad( double t ) {
            return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
        }

        bool mVisible;
        int mHeaderHeight;
        int mFooterHeight;
    
        enum {
            HIDDEN,
            FADING_IN,
            VISIBLE,
            FADING_OUT
        } mState;
    
        struct{
            int x;
            int y;
            int width;
            int height;
            int padding;
            ofColor color;
        } mModal;
    
        struct{
            int nTicks;
            int tTicks;
            float percent;
            float nOpacity;
            float tOpacity;
        } mAnimation;
    
        struct{
            int x;
            int y;
            int height;
            string text;
            ofColor color;
            ofxModalTheme::font* font;
        } mTitle;
    
        struct{
            ofPoint p1;
            ofPoint p2;
        } mLine1;
    
        struct{
            int height;
            bool visible;
            ofxParagraph text;
        } mMessage;
    
        struct{
            ofPoint p1;
            ofPoint p2;
        } mLine2;
    
        struct {
            bool mouseOver;
            int hitPadding;
            ofImage* btnDefault;
            ofImage* btnOnHover;
            ofRectangle rect;
            ofRectangle hitRect;
        } mCloseButton;
    
        vector<ofxDatGuiButton*> mFooterButtons;
    
    /*
        event subscribers
    */
    
        typedef std::function<void(ModalEvent)> onModalEventCallback;
        struct subscriber{
            ModalEvent::EventType eType;
            onModalEventCallback callback;
        };
        vector<subscriber> subscribers;
    
    /*
        static properties & methods
    */
    
        static ofxModal* activeModal;
        static vector<ofxModal*> modals;
        static std::shared_ptr<ofxModalTheme> mTheme;
    
};


class ofxModalConfirm : public ofxModal {

    public:

        ofxModalConfirm()
        {
            setSize(800, 500);
            setTitle("CONFIRM");
            setMessage("This is a confirm message. Stumptown street art photo booth try-hard cold-pressed, pour-over raw denim four loko vinyl. Banjo drinking vinegar tousled, Brooklyn Neutra meggings mlkshk freegan whatever.");
            addButtons();
        }
    
        void draw()
        {
            cancel->draw();
            confirm->draw();
        }
    
        void update()
        {
            cancel->update();
            confirm->update();
        }
    
    protected:
    
        ofxDatGuiButton* cancel;
        ofxDatGuiButton* confirm;
    
    private:
    
        void addButtons()
        {
            cancel = new ofxDatGuiButton("CANCEL");
            cancel->setWidth(140);
            cancel->setStripeVisible(false);
            cancel->setLabelColor(ofColor::fromHex(0x333333));
            cancel->setBackgroundColors(ofColor::fromHex(0xffffff), ofColor::fromHex(0xE6E6E6), ofColor::fromHex(0xD4D4D4));
            cancel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
            cancel->onButtonEvent(this, &ofxModalConfirm::onButtonEvent);
            confirm = new ofxDatGuiButton("OK");
            confirm->setWidth(140);
            confirm->setStripeVisible(false);
            confirm->setLabelColor(ofColor::fromHex(0xffffff));
            confirm->setBackgroundColors(ofColor::fromHex(0x337ab7), ofColor::fromHex(0x286090), ofColor::fromHex(0x1f4c73));
            confirm->setLabelAlignment(ofxDatGuiAlignment::CENTER);
            confirm->onButtonEvent(this, &ofxModalConfirm::onButtonEvent);
            attachFooterButton(confirm);
            attachFooterButton(cancel);
        }
    
        void onButtonEvent(ofxDatGuiButtonEvent e)
        {
            cout << "button clicked" << endl;
            if (e.target == cancel){
                hide();
                dispatchCallbacks(ModalEvent::CANCEL);
            }
        }
    

};


