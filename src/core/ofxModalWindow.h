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

class ofxModalWindow {
    
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
                ofAddListener(ofEvents().draw, this, &ofxModalWindow::onDraw);
                ofAddListener(ofEvents().update, this, &ofxModalWindow::onUpdate);
                ofAddListener(ofEvents().mouseMoved, this, &ofxModalWindow::onMouseMove);
                ofAddListener(ofEvents().mousePressed, this, &ofxModalWindow::onMousePress);
                ofAddListener(ofEvents().windowResized, this, &ofxModalWindow::onWindowResize);
            }
        }
    
        void hide()
        {
            mVisible = true;
            mState = FADING_OUT;
            mAnimation.nTicks = 0;
            mAnimation.percent = 0;
        }
    
        void setWidth(int w)
        {
            mModal.width = w;
            mModal.x = ofGetWidth() / 2 - mModal.width / 2;
            mMessage.setWidth(mModal.width - (mModal.padding * 2));
        }
    
        void setHeight(int h)
        {
            mModal.autoSize = false;
            mModal.height.body = h - mModal.height.header - mModal.height.footer;
        // establish a minimun body height //
            if (mModal.height.body < 200) mModal.height.body = 200;
        }
    
        void setTitle(string text)
        {
            mTitle.text = ofToUpper(text);
            mTitle.height = mTitle.font->height(mTitle.text);
        }
    
        void setMessage(string text)
        {
            mMessage.setText(text);
            mMessageVisible = true;
            if (mModal.autoSize) mModal.height.body = mMessage.getHeight() + mModal.padding * 2;
        }
    
        virtual void setTheme(std::shared_ptr<ofxModalTheme> theme)
        {
            mColor.title = theme->color.text.title;
            mColor.header = theme->color.modal.header;
            mColor.body = theme->color.modal.body;
            mColor.footer = theme->color.modal.footer;
            mColor.hrule = theme->color.modal.hrule;
            mCloseButton.normal = &theme->close_button.normal;
            mCloseButton.active = &theme->close_button.active;
            mCloseButton.rect.width = theme->close_button.width;
            mCloseButton.rect.height = theme->close_button.height;
            mCloseButton.hitPadding = theme->close_button.hitPadding;
            mAnimation.tTicks = theme->animation.speed * ofGetFrameRate();
            mAnimation.tOpacity = theme->alpha.window.background;
            mTitle.font = theme->fonts.title;
            mMessage.setFont(theme->fonts.message);
            mMessage.setColor(theme->color.text.body);
            mMessage.setSpacing(theme->layout.text.wordSpacing);
            mModal.height.header = mModal.padding * 2 + mCloseButton.rect.height;
            mModal.height.footer = mModal.padding * 3;
            mModal.height.body = theme->layout.modal.height - mModal.height.header - mModal.height.footer;
            mModal.padding = theme->layout.modal.padding;
            mModal.vMargin = theme->layout.modal.vMargin;
            mModal.autoSize = theme->layout.modal.autoSize;
            setWidth(theme->layout.modal.width);
        }
    
        int getWidth() { return mModal.width; }
        int getHeight() { return mModal.height.header + mModal.height.body + mModal.height.footer; }
        int getPadding() { return mModal.padding; }
        static bool visible() { return activeModal != nullptr; }
    
        template<typename T, typename args, class ListenerClass>
        void onModalEvent(ofxModalEvent::EventType event, T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            subscribers.push_back({event, std::bind(listenerMethod, owner, _1)});
        }

    
    protected:
    
        ofxModalWindow()
        {
            modals.push_back(this);
            if (mTheme == nullptr) mTheme = std::make_shared<ofxModalTheme>();
            setTheme(mTheme);
            mCloseButton.mouseOver = false;
        }
    
        virtual void onButtonEvent(ofxDatGuiButtonEvent e) = 0;
    
    /*
        modal components
    */
    
        struct ModalComponent{
            int x;
            int y;
            ofxDatGuiComponent* component;
            ModalComponent(ofxDatGuiComponent* c, int x, int y){
                this->x = x;
                this->y = y;
                this->component = c;
            }
        };
    
        template<class component>
        component* attach(component* c, int x = 0, int y = 0, int w = 0)
        {
            if (y == 0) {
                y = mModal.padding;
                for(auto mc:mModalComponents) y+= mc.component->getHeight() + mModal.vMargin;
            }
            mModalComponents.push_back(ModalComponent(c, x, y));
            int maxW = mModal.width-(mModal.padding*2)-x;
            if (w > 0 && w < maxW) {
                c->setWidth(w, .3);
            }   else{
                c->setWidth(maxW, .3);
            }
            if (mModal.autoSize){
                cout << "ok" << endl;
                int h = mModal.padding;
                for(auto mc:mModalComponents) h+= mc.component->getHeight() + mModal.vMargin;
                mModal.height.body = h + mModal.padding;
            }
            return c;
        }
    
        void autoSize()
        {
            for(auto mc:mModalComponents) mc.component->setWidth(mModal.width-(mModal.padding*2)-mc.x, .3);
        }
    
        void autoSize(ofxDatGuiComponent* c)
        {
            for(auto mc:mModalComponents) if (mc.component == c) c->setWidth(mModal.width-(mModal.padding*2)-mc.x, .3);
        }
    
    /*
        footer buttons
    */
    
        void addButton(string label)
        {
            ofxDatGuiButton* btn = new ofxDatGuiButton(ofToUpper(label));
            btn->setStripeVisible(false);
            btn->setLabelAlignment(ofxDatGuiAlignment::CENTER);
            btn->onButtonEvent(this, &ofxModalWindow::onButtonEvent);
            mFooterButtons.push_back(btn);
        }
    
        ofxDatGuiButton* getButton(string label)
        {
            for (auto button:mFooterButtons){
                if (button->is(label)) return button;
            }
            return nullptr;
        }
    
        void dispatchCallbacks(ofxModalEvent::EventType eType)
        {
            for(auto s: subscribers){
                if (s.eType == eType){
                    s.callback(ofxModalEvent(eType, this));
                }
            }
        }
    
        static std::shared_ptr<ofxModalTheme> mTheme;

    private:
    
        void onDraw(ofEventArgs &e)
        {
            ofPushStyle();
        // draw background blackout //
            ofSetColor(0, 0, 0, mAnimation.nOpacity);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        // draw modal header //
            ofSetColor(mColor.header);
            ofDrawRectangle(mModal.x, mModal.y, mModal.width, mModal.height.header);
        // draw modal body //
            ofSetColor(mColor.body);
            ofDrawRectangle(mModal.x, mModal.y + mModal.height.header, mModal.width, mModal.height.body);
        // draw modal header //
            ofSetColor(mColor.footer);
            ofDrawRectangle(mModal.x, mModal.y + mModal.height.header + mModal.height.body, mModal.width, mModal.height.footer);
        // draw title //
            ofSetColor(mColor.title);
            mTitle.font->draw(mTitle.text, mTitle.x, mTitle.y);
            ofDrawLine(mBreak1.p1, mBreak1.p2);
            ofDrawLine(mBreak2.p1, mBreak2.p2);
        // draw message //
            if (mMessageVisible) mMessage.draw();
        // draw close button //
            ofSetColor(ofColor::white);
            if (mCloseButton.mouseOver == false){
                mCloseButton.normal->draw(mCloseButton.rect);
            }   else{
                mCloseButton.active->draw(mCloseButton.rect);
            }
            ofPopStyle();
        // draw body components //
            for(auto mc:mModalComponents) mc.component->draw();
        // draw footer buttons //
            for(auto button:mFooterButtons) button->draw();
        }
    
        void onUpdate(ofEventArgs &e)
        {
            if (mState == FADING_IN || mState == FADING_OUT){
                animate();
            }   else{
            // update modal components //
                for(auto bn:mFooterButtons) bn->update();
                for(auto mc:mModalComponents) mc.component->update();
            }
        }
    
        inline void layout()
        {
            mTitle.x = mModal.x + mModal.padding;
            mTitle.y = mModal.y + mModal.padding + mCloseButton.rect.height/2 + mTitle.height/2;
            mBreak1.p1.x = mModal.x;
            mBreak1.p1.y = mModal.y + mModal.height.header;
            mBreak1.p2.x = mModal.x + mModal.width;
            mBreak1.p2.y = mBreak1.p1.y;
            mBreak2.p1.x = mBreak1.p1.x;
            mBreak2.p1.y = mModal.y + mModal.height.header + mModal.height.body;
            mBreak2.p2.x = mBreak1.p2.x;
            mBreak2.p2.y = mBreak2.p1.y;
            mMessage.setPosition(mBreak1.p1.x + mModal.padding, mBreak1.p1.y + mModal.padding + mMessage.getStringHeight());
            mCloseButton.rect.x = mModal.x + mModal.width - mModal.padding - mCloseButton.rect.width;
            mCloseButton.rect.y = mModal.y + mModal.padding;
            mCloseButton.hitRect.x = mCloseButton.rect.x - mCloseButton.hitPadding;
            mCloseButton.hitRect.y = mCloseButton.rect.y - mCloseButton.hitPadding;
            mCloseButton.hitRect.width = mCloseButton.rect.width + (mCloseButton.hitPadding * 2);
            mCloseButton.hitRect.height = mCloseButton.rect.height + (mCloseButton.hitPadding * 2);
            for(auto mc:mModalComponents) {
                mc.component->setPosition(mModal.x + mModal.padding + mc.x, mModal.y + mModal.height.header + mc.y);
            }
            for(int i=0; i<mFooterButtons.size(); i++){
                int buttonSpacing = 8;
                int w = mFooterButtons[0]->getWidth();
                int x = mModal.x + mModal.width - mModal.padding - w;
                int y = mBreak2.p1.y + mModal.height.footer/2 - mFooterButtons[i]->getHeight()/2;
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
            int height = getHeight();
            mAnimation.nOpacity = mAnimation.percent * (mAnimation.tOpacity * 255);
            mModal.y = -height + mAnimation.percent * (ofGetHeight()/2 - height/2 + height);
            if (mAnimation.nTicks == mAnimation.tTicks){
                if (mState == FADING_IN){
                    mState = VISIBLE;
                    dispatchCallbacks(ofxModalEvent::SHOWN);
                }   else if (mState == FADING_OUT){
                    mState = HIDDEN;
                // modal is closed, ok to show another one now //
                    activeModal = nullptr;
                    dispatchCallbacks(ofxModalEvent::HIDDEN);
                    ofRemoveListener(ofEvents().draw, this, &ofxModalWindow::onDraw);
                    ofRemoveListener(ofEvents().update, this, &ofxModalWindow::onUpdate);
                    ofRemoveListener(ofEvents().mouseMoved, this, &ofxModalWindow::onMouseMove);
                    ofRemoveListener(ofEvents().mousePressed, this, &ofxModalWindow::onMousePress);
                }
            }
        // sync modal components as window moves //
            layout();
        }
    
        void onMousePress(ofMouseEventArgs &e)
        {
            ofPoint mouse = ofPoint(e.x, e.y);
            if (ofRectangle(mModal.x, mModal.y, mModal.width, getHeight()).inside(mouse) == false) {
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
            int height = getHeight();
            mModal.x = ofGetWidth() / 2 - mModal.width / 2;
            mModal.y = -height + mAnimation.percent * (ofGetHeight()/2 - height/2 + height);
        }
    
        double easeInOutQuad( double t ) {
            return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
        }

        bool mVisible;
        bool mMessageVisible;
    
        enum {
            HIDDEN,
            FADING_IN,
            VISIBLE,
            FADING_OUT
        } mState;
    
        struct {
            ofColor title;
            ofColor header;
            ofColor body;
            ofColor hrule;
            ofColor footer;
        } mColor;
    
        struct{
            int x;
            int y;
            int width;
            int padding;
            int vMargin;
            struct{
                int header;
                int body;
                int footer;
            } height;
            bool autoSize;
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
            shared_ptr<ofxSmartFont> font;
        } mTitle;
    
        struct{
            ofPoint p1;
            ofPoint p2;
        } mBreak1;
    
        struct{
            ofPoint p1;
            ofPoint p2;
        } mBreak2;
    
        struct {
            bool mouseOver;
            int hitPadding;
            ofImage* normal;
            ofImage* active;
            ofRectangle rect;
            ofRectangle hitRect;
        } mCloseButton;
    
        ofxParagraph mMessage;
        vector<ModalComponent> mModalComponents;
        vector<ofxDatGuiButton*> mFooterButtons;
    
    /*
        event subscribers
    */
    
        typedef std::function<void(ofxModalEvent)> onModalEventCallback;
        struct subscriber{
            ofxModalEvent::EventType eType;
            onModalEventCallback callback;
        };
        vector<subscriber> subscribers;
    
    /*
        static properties & methods
    */
    
        static ofxModalWindow* activeModal;
        static vector<ofxModalWindow*> modals;
    
};

