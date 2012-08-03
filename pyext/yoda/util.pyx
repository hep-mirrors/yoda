from collections import namedtuple

cdef class Base:
    pass

def try_loop(fs, *args, char *_msg='Invalid arguments', **kwargs):
    for f in fs:
        try:
            f(*args, **kwargs)
            return
        except TypeError:
            pass

    raise TypeError(_msg)

Edges = namedtuple('Edges', ('low', 'high'))
Errors = namedtuple('Errors', ('minus', 'plus'))
XY = namedtuple('XY', ('x', 'y'))
XYZ = namedtuple('XYZ', ('x', 'y', 'z'))
