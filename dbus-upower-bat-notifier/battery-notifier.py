#!/usr/bin/env python3

from gi.repository import GLib
import dbus
import dbus.mainloop.glib
import sys

dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

bus = dbus.SystemBus()

upower = bus.get_object('org.freedesktop.UPower', '/org/freedesktop/UPower')
devices = upower.EnumerateDevices(dbus_interface='org.freedesktop.UPower')

battery_path = ''

for d in devices:
    name = d.split('/')[-1]
    if name.startswith('battery_'):
        print('using battery %s' % name[len('battery_'):])
        battery_path = d
        break

if not battery_path:
    print('could not find a battery')
    sys.exit(1)


def notify_battery_percentage(percentage):
    print('battery is at %d percent' % percentage)


def signal_handler(iface, properties, _):
    if 'Percentage' in properties:
        percentage = properties['Percentage']
        print('battery is at %d percent' % percentage)


battery = bus.get_object('org.freedesktop.UPower', battery_path)

battery_properties = dbus.Interface(battery, 'org.freedesktop.DBus.Properties')

percentage = battery_properties.Get('org.freedesktop.UPower.Device', 'Percentage')

notify_battery_percentage(percentage)

battery_properties.connect_to_signal('PropertiesChanged', signal_handler, dbus_interface='org.freedesktop.DBus.Properties')

loop = GLib.MainLoop()
loop.run()
