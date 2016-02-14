##ofxModal

A flexible and extensible kit of Modal windows for [openFrameworks](http://openframeworks.cc/).

![ofxModalAlert](./readme-imgs/ofxModalAlert.gif)

## Installation

ofxModal is built on top of [ofxDatGui](https://github.com/braitsch/ofxDatGui) and [ofxParagraph](https://github.com/braitsch/ofxParagraph) which requires you to clone the repository recursively if you do not already have these addons installed.

	git clone --recursive git@github.com:braitsch/ofxModal.git


Once you've created a project copy the **ofxbraitsch** directory in the root of this repository to your project's bin/data directory. This directory contains the fonts & icons used by ofxModal & ofxDatGui.

## Alerts
Displaying an Alert is as simple as:

	ofxModalAlert myAlert;
	myAlert.alert("you have borked the system.");

However it's a good idea to create a single application Alert that can be shared across multiple modal windows.

	myCustomModal myModal;
	shared_ptr<ofxModalAlert> myAlert = make_shared<ofxModalAlert>();
	myModal.setAlert(myAlert);
	myModal.show();
	myModal.alert("hi it's me again!");
	
This will queue the Alert to show after your custom modal window has closed.

## Confirms
Confirm windows require a little more setup because you typically will want to do something based on the user's response.

	ofxModalConfirm confirm;
	confirm.addListener(this, &ofApp::onModalEvent);
	confirm.setMessage("Are you sure you really want to do this?");
	confirm.show();
	
	void onModalEvent(ofxModalEvent e)
	{
		if (e.type == ofxModalEvent::CANCEL){
			cout << "cancel button was selected" << endl;
		else if (e.type == ofxModalEvent::CONFIRM){
			cout << "confirm button was selected" << endl;
		}
	}

## Custom Modals

	class myCustomModal : public ofxModalConfirm
	{
		...
	};

## Appending Alerts
As described above it's a good idea to create a single Alert for your entire application and then assign it to each of your modals to display should an interaction in your modal warrant an alert.

	myCustomModal myModal;
	shared_ptr<ofxModalAlert> myAlert = make_shared<ofxModalAlert>();

	myModal.setAlert(myAlert);
	myModal.addListener(this, &ofApp::onModalEvent);
	
	void onModalEvent(ofxModalEvent e)
	{
		if (e.type == ofxModalEvent::CONFIRM){
		// query your custom modal for valid user data //
			if (myModal.hasValidUserData() == false){
				myModal.alert("Nope. Invalid Response");
			}
		}
	}

![ofxModalAlert](./readme-imgs/ofxModalAutoAlert.gif)

## API

	void show();
	void hide();
	void setWidth(int width);
	void setHeight(int height);
	void setTitle(string text);
	void setMessage(string text);
	void setMessageAlignment(ofxParagraph::Alignment align);
	void setBackdropActive(bool active); // close window if backdrop is clicked //

## Customization

[ofxModalTheme.h](https://github.com/braitsch/ofxModal/blob/master/src/themes/ofxModalTheme.h)