#
# Most definitely not a good idea, but useful as a warning
#
#from libc.stdlib cimport malloc
#


#ctypedef map[string,string].iterator mapiter
#
#cdef class Annotations_itemiterator():
#    cdef mapiter *it
#
#    def __cinit__(self):
#        self.it = <mapiter *> malloc(sizeof(mapiter))
#
#
#cdef class Annotations(util.Base):
#    cdef c.AnalysisObject *_AnalysisObject(self) except NULL:
#        return <c.AnalysisObject *> self.ptr()
#
#
#    def __getitem__(self, char *key):
#        return self._AnalysisObject().annotation(string(key)).c_str()
#
#    def __setitem__(self, char *key, char *value):
#        self._AnalysisObject().setAnnotation(string(key), string(value))
#    
#    # Doesn't actually iterate...
#    def iteritems(self):
#        it = self.it
#        it[0] = self._AnalysisObject().annotations().begin()
#        
#        while True:
#            yield deref(it[0]).first.c_str(), deref(it[0]).second.c_str()
#            preinc(it[0])
#            if it[0] == self._AnalysisObject().annotations().end():
#                return
#
#    def __repr__(self):
#        return '{%s}' % ', '.join('%r: %r' % i for i in self.iteritems())
