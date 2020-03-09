#GST_PLUGIN_PATH=. gst-launch-1.0 numbersrc ! numbersink

#GST_DEBUG=4 GST_PLUGIN_PATH=. gst-launch-1.0 rs2src ! fakesink

gst-launch-1.0 -v videotestsrc ! video/x-raw,framerate=30/1,width=640,height=480,format=RGB ! videoconvert ! ximagesink
#GST_PLUGIN_PATH=. gst-launch-1.0 -v rs2src ! video/x-raw,framerate=30/1,width=640,height=480,format=RGB ! videoconvert ! ximagesink
