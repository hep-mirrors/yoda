## Pull in core YODA C++/Python extension functionality
from yoda.core import *

## Try to pull in plotting tools in a non-dependency-inducing way
def plot(*args, **kwargs):
    from yoda.plotting import plot as p
    return p(*args, **kwargs)

## Try to pull in optional ROOT compatibility
try:
    from yoda.rootcompat import *
except:
    pass
