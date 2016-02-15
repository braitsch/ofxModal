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
            addComponent(new ofxDatGuiTextInput("username", "username"));
            addComponent(new ofxDatGuiTextInput("password", "password"));
        }
        
};