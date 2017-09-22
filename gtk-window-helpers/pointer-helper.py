#!/usr/bin/env python3

import gi
import cgi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GLib, Gdk
import signal

signal.signal(signal.SIGINT, signal.SIG_DFL)

class InfoWindow(Gtk.Window):

	def __init__(self):
		super().__init__()

		self.monospace = lambda s: '<span font="monospace">{}</span>'.format(cgi.escape(s))

		self.connect('destroy', Gtk.main_quit)
		self.props.title = 'test-window'

		self.box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
		self.add(self.box)

		self.motion_label = Gtk.Label()
		self.motion_label.set_markup(self.monospace('waiting for motion...'))
		self.box.pack_start(self.motion_label, True, True, 0)

		self.enter_label = Gtk.Label()
		self.enter_label.set_markup(self.monospace('waiting for enter...'))
		self.box.pack_start(self.enter_label, True, True, 0)

		self.button_label = Gtk.Label()
		self.button_label.set_markup(self.monospace('waiting for button...'))
		self.box.pack_start(self.button_label, True, True, 0)

		self.scroll_label = Gtk.Label()
		self.scroll_label.set_markup(self.monospace('waiting for scroll...'))
		self.box.pack_start(self.scroll_label, True, True, 0)

		self.connect('button-press-event', self.button_handler)
		self.connect('button-release-event', self.button_handler)
		self.connect('enter-notify-event', self.enter_handler)
		self.connect('leave-notify-event', self.enter_handler)
		self.connect('motion-notify-event', self.motion_handler)
		self.connect('scroll-event', self.scroll_handler)

		self.show_all()
		self.props.window.set_events(Gdk.EventMask.ALL_EVENTS_MASK)

	def button_handler(self, widget, e):
		x = round(e.x, 4)
		y = round(e.y, 4)
		etype = 'press' if e.type == Gdk.EventType.BUTTON_PRESS else 'release'
		text = 'button {}: x={} y={} button={}'.format(etype, x, y, e.button)
		self.button_label.set_markup(self.monospace(text))

	def enter_handler(self, widget, e):
		x = round(e.x, 4)
		y = round(e.y, 4)
		etype = 'enter' if e.type == Gdk.EventType.ENTER_NOTIFY else 'leave'
		text = '{}: x={} y={}'.format(etype, x, y)
		self.enter_label.set_markup(self.monospace(text))

	def motion_handler(self, widget, e):
		x = round(e.x, 4)
		y = round(e.y, 4)
		text = 'motion: x={} y={}'.format(x, y)
		self.motion_label.set_markup(self.monospace(text))

	def scroll_handler(self, widget, e):
		x = round(e.x, 4)
		y = round(e.y, 4)
		dx = round(e.delta_x, 4)
		dy = round(e.delta_y, 4)
		text = 'axis: x={} y={} dx={} dy={}'.format(x, y, dx, dy)
		self.scroll_label.set_markup(self.monospace(text))

w = InfoWindow()

Gtk.main()
