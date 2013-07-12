## Pull in core YODA C++/Python extension functionality
from yoda.core import *

## Try to pull in optional ROOT compatibility
try:
    from yoda.rootcompat import *
except:
    pass
