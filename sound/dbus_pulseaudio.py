#!/usr/bin/env python

import dbus
import os

def connect():
    if 'PULSE_DBUS_SERVER' in os.environ:
        address = os.environ['PULSE_DBUS_SERVER']
    else:
        bus = dbus.SessionBus()
        server_lookup = bus.get_object("org.PulseAudio1", "/org/pulseaudio/server_lookup1")
        address = server_lookup.Get("org.PulseAudio.ServerLookup1", "Address", dbus_interface="org.freedesktop.DBus.Properties")

    return dbus.connection.Connection(address)


conn = connect()
core = conn.get_object(object_path="/org/pulseaudio/core1")

print "Successfully connected to " + core.Get("org.PulseAudio.Core1", "Name", dbus_interface="org.freedesktop.DBus.Properties") + "!"
