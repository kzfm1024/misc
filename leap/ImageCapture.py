################################################################################
# Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.               #
# Leap Motion proprietary and confidential. Not for distribution.              #
# Use subject to the terms of the Leap Motion SDK Agreement available at       #
# https://developer.leapmotion.com/sdk_agreement, or another agreement         #
# between Leap Motion and you, your company or other organization.             #
################################################################################

import os, sys, inspect
src_dir = os.path.dirname(inspect.getfile(inspect.currentframe()))
lib_dir = os.path.abspath(os.path.join(src_dir, './lib'))
sys.path.insert(0, lib_dir)

import Leap, thread, time
import Image

counter = 0

class SampleListener(Leap.Listener):

    #def __init__(self):
    #self.counter = 0

    def on_init(self, controller):
        print "Initialized"

    def on_connect(self, controller):
        print "Connected"

    def on_disconnect(self, controller):
        # Note: not dispatched when running in a debugger.
        print "Disconnected"

    def on_exit(self, controller):
        print "Exited"

    def on_frame(self, controller):
        global counter
        frame = controller.frame()
        images = frame.images
        self.save(images[0], 'left%03d.pgm' % counter)
        self.save(images[1], 'right%03d.pgm' % counter)
        counter = counter + 1

    def save(self, image, filename):
        ptr = image.data
        w = image.width
        h = image.height
        img = Image.new('L', (w, h), (0))
        for y in range(h):
            for x in range(w):
                pix = ptr[y * w + x]
                img.putpixel((x, y), (pix))
        img.save(filename)                

def main():
    # Create a sample listener and controller
    listener = SampleListener()
    controller = Leap.Controller()

    controller.set_policy_flags(Leap.Controller.POLICY_IMAGES)

    # Have the sample listener receive events from the controller
    controller.add_listener(listener)

    # Keep this process running until Enter is pressed
    print "Press Enter to quit..."
    try:
        sys.stdin.readline()
    except KeyboardInterrupt:
        pass
    finally:
        # Remove the sample listener when done
        controller.remove_listener(listener)


if __name__ == "__main__":
    main()
