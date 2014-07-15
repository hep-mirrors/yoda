# from collections import namedtuple
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


# TODO: remove
class Edges(tuple):
    'Edges(low, high)'

    __slots__ = ()

    _fields = ('low', 'high')

    def __new__(_cls, low, high):
        'Create new instance of Edges(low, high)'
        return tuple.__new__(_cls, (low, high))

    @classmethod
    def _make(cls, iterable, new=tuple.__new__, len=len):
        'Make a new Edges object from a sequence or iterable'
        result = new(cls, iterable)
        if len(result) != 2:
            raise TypeError('Expected 2 arguments, got %d' % len(result))
        return result

    def __repr__(self):
        'Return a nicely formatted representation string'
        return 'Edges(low=%r, high=%r)' % self

    def _asdict(self):
        'Return a new dict which maps field names to their values'
        return dict(zip(self._fields, self))

    __dict__ = property(_asdict)

    def _replace(_self, **kwds):
        'Return a new Edges object replacing specified fields with new values'
        result = _self._make(map(kwds.pop, ('low', 'high'), _self))
        if kwds:
            raise ValueError('Got unexpected field names: %r' % kwds.keys())
        return result

    def __getnewargs__(self):
        'Return self as a plain tuple.  Used by copy and pickle.'
        return tuple(self)

    low = property(itemgetter(0), doc='Alias for field number 0')
    high = property(itemgetter(1), doc='Alias for field number 1')


class Errors(tuple):
    'Errors(minus, plus)'

    __slots__ = ()

    _fields = ('minus', 'plus')

    def __new__(_cls, minus, plus):
        'Create new instance of Errors(minus, plus)'
        return tuple.__new__(_cls, (minus, plus))

    @classmethod
    def _make(cls, iterable, new=tuple.__new__, len=len):
        'Make a new Errors object from a sequence or iterable'
        result = new(cls, iterable)
        if len(result) != 2:
            raise TypeError('Expected 2 arguments, got %d' % len(result))
        return result

    def __repr__(self):
        'Return a nicely formatted representation string'
        return 'Errors(minus=%r, plus=%r)' % self

    def _asdict(self):
        'Return a new dict which maps field names to their values'
        return dict(zip(self._fields, self))

    __dict__ = property(_asdict)

    def _replace(_self, **kwds):
        'Return a new Errors object replacing specified fields with new values'
        result = _self._make(map(kwds.pop, ('minus', 'plus'), _self))
        if kwds:
            raise ValueError('Got unexpected field names: %r' % kwds.keys())
        return result

    def __getnewargs__(self):
        'Return self as a plain tuple.  Used by copy and pickle.'
        return tuple(self)

    minus = property(itemgetter(0), doc='Alias for field number 0')
    plus = property(itemgetter(1), doc='Alias for field number 1')


#Edges = namedtuple('Edges', ('low', 'high'))
#Errors = namedtuple('Errors', ('minus', 'plus'))


class XY(tuple):
    'XY(x, y)'

    __slots__ = ()

    _fields = ('x', 'y')

    def __new__(_cls, x, y):
        'Create new instance of XY(x, y)'
        return tuple.__new__(_cls, (x, y))

    @classmethod
    def _make(cls, iterable, new=tuple.__new__, len=len):
        'Make a new XY object from a sequence or iterable'
        result = new(cls, iterable)
        if len(result) != 2:
            raise TypeError('Expected 2 arguments, got %d' % len(result))
        return result

    def __repr__(self):
        'Return a nicely formatted representation string'
        return 'XY(x=%r, y=%r)' % self

    def _asdict(self):
        'Return a new dict which maps field names to their values'
        return dict(zip(self._fields, self))

    __dict__ = property(_asdict)

    def _replace(_self, **kwds):
        'Return a new XY object replacing specified fields with new values'
        result = _self._make(map(kwds.pop, ('x', 'y'), _self))
        if kwds:
            raise ValueError('Got unexpected field names: %r' % kwds.keys())
        return result

    def __getnewargs__(self):
        'Return self as a plain tuple.  Used by copy and pickle.'
        return tuple(self)

    x = property(itemgetter(0), doc='Alias for field number 0')
    y = property(itemgetter(1), doc='Alias for field number 1')


#XYZ = namedtuple('XYZ', ('x', 'y', 'z'))
class XYZ(tuple):
    'XYZ(x, y, z)'

    __slots__ = ()

    _fields = ('x', 'y', 'z')

    def __new__(_cls, x, y, z):
        'Create new instance of XYZ(x, y, z)'
        return tuple.__new__(_cls, (x, y, z))

    @classmethod
    def _make(cls, iterable, new=tuple.__new__, len=len):
        'Make a new XYZ object from a sequence or iterable'
        result = new(cls, iterable)
        if len(result) != 3:
            raise TypeError('Expected 3 arguments, got %d' % len(result))
        return result

    def __repr__(self):
        'Return a nicely formatted representation string'
        return 'XYZ(x=%r, y=%r, z=%r)' % self

    def _asdict(self):
        'Return a new dict which maps field names to their values'
        return dict(zip(self._fields, self))

    __dict__ = property(_asdict)

    def _replace(_self, **kwds):
        'Return a new XYZ object replacing specified fields with new values'
        result = _self._make(map(kwds.pop, ('x', 'y', 'z'), _self))
        if kwds:
            raise ValueError('Got unexpected field names: %r' % kwds.keys())
        return result

    def __getnewargs__(self):
        'Return self as a plain tuple.  Used by copy and pickle.'
        return tuple(self)

    x = property(itemgetter(0), doc='Alias for field number 0')
    y = property(itemgetter(1), doc='Alias for field number 1')
    z = property(itemgetter(2), doc='Alias for field number 2')
