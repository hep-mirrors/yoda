#! /usr/bin/env python
from __future__ import print_function
from yoda import *
h = Histo1D("/foo", "Title", 50, 0.0, 100.0)
print(h.bins())
