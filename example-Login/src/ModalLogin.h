//
//  ModalForm.h
//  example-Custom
//
//  Created by Stephen Braitsch on 2/13/16.
//
//

#pragma once
#include "ofxModal.h"

class ModalLogin : public ofxModalWindow{

    public:
    
        ModalLogin(){
            setTitle("Login");
            addComponent(new ofxDatGuiTextInput("username", "username"))->setTheme(&guiTheme);
            addComponent(new ofxDatGuiTextInput("password", "password"))->setTheme(&guiTheme);
            addButton("Cancel");
            getButton(0)->setLabel("Submit");
            getButton(0)->setLabelColor(mTheme->color.button.darkblue.label);
            getButton(0)->setBackgroundColors(mTheme->color.button.darkblue.background,
                mTheme->color.button.darkblue.backgroundOnMouseOver, mTheme->color.button.darkblue.backgroundOnMouseDown);
            getButton(0)->setBorder(mTheme->color.button.darkblue.border, 1);
            autoSize();
        }
    
    private:
    
        ofxDatGuiThemeCharcoal guiTheme;
        
};