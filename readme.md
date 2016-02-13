##ofxModal

A flexible and extensible kit of Modal windows for [openFrameworks](http://openframeworks.cc/).

...

## Alerts
Displaying an Alert is as simple as:

	ofxModalAlert myAlert;
	myAlert.alert("you have borked the system.");
	
Although it's a good idea to create a single Alert for your entire application which you can then assign to other modal windows in the event they generate an error. 

	myCustomModal myModal;
	shared_ptr<ofxModalAlert> myAlert = make_shared<ofxModalAlert>();
	myModal.setAlert(myAlert);
	myModal.alert("hi it's me again!");
	
This automatically delays showing the Alert until after the offending window has closed eliminating the need for you to stagger them out yourself.

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
As described above it's a good idea to create a single Alert for your entire application and then assign it to each of your modals to display in the event they encounter an error.

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

## Customization

	void setWidth(int width);
	void setHeight(int height);
	void setTitle(string text);
	void setMessage(string text);
	void setBackdropActive(bool active);

	
