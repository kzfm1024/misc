#!/usr/bin/env python

import dbus

def print_dictionary(dict):
    for x in dict:
        print('  {key} : {value}'.format(key=x, value=dict[x]))

def print_array(arr):
    for x in arr:
        print(x)

def print_technologies(technologies):
    for tech in technologies:
        for x in tech:
            if isinstance(x, dbus.Dictionary):
                print_dictionary(x)
            else:
                print(x)

def print_services(services):
    for tech in services:
        for x in tech:
            if isinstance(x, dbus.Dictionary):
                print_dictionary(x)
            else:
                print(x)
            
def main():
    bus = dbus.SystemBus()
    obj = bus.get_object('net.connman', '/')
    manager = dbus.Interface(obj, dbus_interface='net.connman.Manager')

    print('[net.connman.Manager Properties]')
    print_dictionary(manager.GetProperties())

    print('[Technologies]')
    print_technologies(manager.GetTechnologies())

    print('[Services]')
    print_services(manager.GetServices())

if __name__ == '__main__':
    main()
