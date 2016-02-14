//
//  ModalForm.h
//  example-Custom
//
//  Created by Stephen Braitsch on 2/13/16.
//
//

#pragma once
#include "ofxModal.h"

class ModalForm : public ofxModalWindow{

    public:
    
        ModalForm(){
            setMessage("hello");
        }
        
};