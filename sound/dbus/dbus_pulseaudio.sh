
dbus-send --print-reply --dest=org.PulseAudio1 /org/pulseaudio/server_lookup1 org.freedesktop.DBus.Properties.Get string:'org.PulseAudio.ServerLookup1' string:'Address'

#
# /org/pulseaudio/core1
#
dbus-send --address=unix:path\=/run/user/1000/pulse/dbus-socket --print-reply /org/pulseaudio/core1 org.freedesktop.DBus.Introspectable.Introspect
dbus-send --address=unix:path\=/run/user/1000/pulse/dbus-socket --print-reply /org/pulseaudio/core1 org.freedesktop.DBus.Properties.GetAll string:"org.PulseAudio.Core1"

#
# /org/pulseaudio/core1/sink0
#
dbus-send --address=unix:path\=/run/user/1000/pulse/dbus-socket --print-reply /org/pulseaudio/core1/sink0 org.freedesktop.DBus.Introspectable.Introspect
dbus-send --address=unix:path\=/run/user/1000/pulse/dbus-socket --print-reply /org/pulseaudio/core1/sink0 org.freedesktop.DBus.Properties.GetAll string:"org.PulseAudio.Core1.Device"

#
# /org/pulseaudio/core1/source0
#
dbus-send --address=unix:path\=/run/user/1000/pulse/dbus-socket --print-reply /org/pulseaudio/core1/source0 org.freedesktop.DBus.Introspectable.Introspect
dbus-send --address=unix:path\=/run/user/1000/pulse/dbus-socket --print-reply /org/pulseaudio/core1/source0 org.freedesktop.DBus.Properties.GetAll string:"org.PulseAudio.Core1.Device"
