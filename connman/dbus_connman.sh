dbus-send --print-reply --system --dest=net.connman / org.freedesktop.DBus.Introspectable.Introspect

dbus-send --print-reply --system --dest=net.connman / net.connman.Manager.GetProperties
dbus-send --print-reply --system --dest=net.connman / net.connman.Clock.GetProperties

#
# /net/connman/service/ethernet_*
#
dbus-send --print-reply --system --dest=net.connman /net/connman/service/ethernet_94de80bd7495_cable org.freedesktop.DBus.Introspectable.Introspect
dbus-send --print-reply --system --dest=net.connman /net/connman/service/ethernet_94de80bd7495_cable net.connman.Service.GetProperties

#
# /net/connman/technology/{ethernet,wifi,bluetooth}
#
dbus-send --print-reply --system --dest=net.connman /net/connman/technology/ethernet org.freedesktop.DBus.Introspectable.Introspect
dbus-send --print-reply --system --dest=net.connman /net/connman/technology/ethernet net.connman.Technology.GetProperties
