from cython.operator cimport dereference as deref, preincrement as preinc
from cStringIO import StringIO

cdef void set_ann(c.AnalysisObject *ana, char *k, char *v):
    ana.setAnnotation(string(k), string(v))

# AnalysisObject is the base class of most of the user facing objects
cdef class AnalysisObject(util.Base):

    # Pointer upcasting mechanism
    cdef inline c.AnalysisObject *_AnalysisObject(self) except NULL:
        return <c.AnalysisObject *> self.ptr()

    # Deallocator (only needed as a base class)
    def __dealloc__(self):
        cdef c.AnalysisObject *p = self._AnalysisObject()
        if self._deallocate:
            del p

    def annotations(self):
        """
        Key value pairs of metadata, returned as a dictionary.
        
        """
        ana = self._AnalysisObject().annotations()
        it = ana.begin()
        out_dict = {}
        while it != ana.end():
            out_dict[deref(it).first.c_str()] = deref(it).second.c_str()
            preinc(it)
        return out_dict

    def string(self):
        """
        A human readable representation of this object as it would be
        stored in a YODA file.

        """
        f = StringIO() 
        writeYODA([self], f)
        f.seek(0)
        return f.read().strip()

    
    def updateAnnotations(self, E=None, **F):
        """
        AO.update([E, ]**F) -> None.  Update annotations of AO from
        dict/iterable E and F.

        If E present and has a .keys() method:
            for k in E: AO[k] = E[k]
        If E present and lacks .keys() method:
            for (k, v) in E: AO[k] = v
        In either case, this is followed by:
            for k in F: AO[k] = F[k]

        """
        AO = self._AnalysisObject()
        if E is not None:
            if hasattr(E, 'keys'):
                for k in E:
                    set_ann(AO, k, E[k])
            else:
                for k, v in E:
                    set_ann(AO, k, v)

        for k in F:
            set_ann(AO, k, F[k])

    property path:
        """
        Used for persistence and as a unique identifier. Must begin with
        a '/' if not the empty string.

        """

        def __get__(self):
            return self._AnalysisObject().path().c_str()

        def __set__(self, char *path):
            self._AnalysisObject().setPath(string(path))

    property title:
        def __get__(self):
            return self._AnalysisObject().title().c_str()

        def __set__(self, char *title):
            self._AnalysisObject().setTitle(string(title))


    def __repr__(self):
        return "<%s '%s'>" % (self.__class__.__name__, self.path)
