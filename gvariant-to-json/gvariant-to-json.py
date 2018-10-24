#!/usr/bin/env python3

import gi
gi.require_version('Json', '1.0')
from gi.repository import Json, GLib
import sys

if len(sys.argv) != 2:
    print("pass a gvariant string as argument 1 (ex: <{'foo':'bar'}>)")
    sys.exit(1)

variant_string = sys.argv[1]

variant = GLib.Variant.parse(GLib.VariantType.new('v'),
                             variant_string, None, None)

data = Json.gvariant_serialize_data(variant)

print(data[0])
