from collections import namedtuple
from operator import itemgetter

def _autotype(var, autobool=True):
    """Automatically convert strings to numerical types if possible."""
    if type(var) is not str:
        return var
    if autobool:
        if var.lower() in ("yes", "true", "on"):
            return True
        if var.lower() in ("no", "false", "off"):
            return False
    if var.isdigit() or (var.startswith("-") and var[1:].isdigit()):
        return int(var)
    try:
        f = float(var)
        return f
    except ValueError:
        return var

def _autostr(var, precision=8):
    """Automatically format numerical types as the right sort of string."""
    if type(var) is float:
        return ("% ." + str(precision) + "e") % var
    elif not hasattr(var, "__iter__"):
        return str(var)
    else:
        return ",".join(_autostr(subval) for subval in var)


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


XY = namedtuple('XY', ('x', 'y'))
XYZ = namedtuple('XYZ', ('x', 'y', 'z'))
EdgePair = namedtuple('EdgePair', ('low', 'high'))
ErrorPair = namedtuple('ErrorPair', ('minus', 'plus'))


## Utils for handling error conversions to/from std::pair
from libcpp.pair cimport pair

def read_edge_pair(pair[double, double] es):
    return EdgePair(es.first, es.second)

def read_error_pair(pair[double, double] es):
    return ErrorPair(es.first, es.second)

def read_symmetric(val):
    try:
        a, b = val
    except TypeError:
        a = b = val
    return pair[double, double](a, b)
