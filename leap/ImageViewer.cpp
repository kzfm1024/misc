/******************************************************************************\
 * Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
 * Leap Motion proprietary and confidential. Not for distribution.              *
 * Use subject to the terms of the Leap Motion SDK Agreement available at       *
 * https://developer.leapmotion.com/sdk_agreement, or another agreement         *
 * between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include <string.h>
#include <fstream>
#include "Leap.h"

#include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

using namespace Leap;

class SampleListener : public Listener {
public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);

private:
	void saveRawImage(const char*, const char*, int);
};

void SampleListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
}

void SampleListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	ImageList images = frame.images();
	if (images.count() > 0) {
		Image img0 = images[0];
		int w0 = img0.width();
		int h0 = img0.height();
		const unsigned char* data0 = img0.data();
		cv::Mat left(h0, w0, CV_8UC1, (void*)data0);

		Image img1 = images[1];
		int w1 = img1.width();
		int h1 = img1.height();
		const unsigned char* data1 = img1.data();
		cv::Mat right(h1, w1, CV_8UC1, (void*)data1);
					 
		cv::imshow("left", left);
		cv::imshow("right", right);
		cv::waitKey(1);
	}
}

void SampleListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void SampleListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}

void SampleListener::saveRawImage(const char* path, const char* data, int size) {
	std::ofstream ofs;
	ofs.open(path, std::ofstream::binary);
	ofs.write(data, size);
	ofs.flush();
	ofs.close();
}

int main(int argc, char** argv) {

	cv::namedWindow("Leap Motion left image");

	// Create a sample listener and controller
	SampleListener listener;
	Controller controller;

	// Have the sample listener receive events from the controller
	controller.addListener(listener);

	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	Controller::PolicyFlag addImagePolicy =
		(Controller::PolicyFlag)(Controller::POLICY_IMAGES|controller.policyFlags());
	controller.setPolicyFlags(addImagePolicy);

	// Keep this process running until Enter is pressed
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	// Remove the sample listener when done
	controller.removeListener(listener);

	cv::destroyWindow("Leap Motion left image");

	return 0;
}
