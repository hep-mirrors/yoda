from cython.operator cimport dereference as deref, preincrement as preinc
from cStringIO import StringIO

# Useful helper function to avoid hoops in Cython's type system
# TODO: Necessary?
cdef void set_annotation(c.AnalysisObject *ana, char *k, char *v):
    ana.setAnnotation(string(k), string(v))


# AnalysisObject is the base class of most of the user facing objects
cdef class AnalysisObject(util.Base):

    # Pointer upcasting mechanism
    cdef inline c.AnalysisObject *_AnalysisObject(self) except NULL:
        return <c.AnalysisObject *> self.ptr()

    # Deallocator (only needed as a base class)
    def __dealloc__(self):
        p = self._AnalysisObject()
        if self._deallocate:
            del p

    @property
    def annotations(self):
        """
        Key value pairs of metadata, returned as a Python dictionary.

        """
        # TODO: It would be best if this was returned by reference so that it could also be *modified* as a dict
        ana = self._AnalysisObject().annotations()
        it = ana.begin()
        out_dict = {}
        while it != ana.end():
            out_dict[deref(it).first.c_str()] = deref(it).second.c_str()
            preinc(it)
        return out_dict

    def updateAnnotations(self, E=None, **F):
        # TODO: Yuck!
        """
        AO.update([E, ]**F) -> None.  Update annotations of AO from
        dict/iterable E and F. Has the same semantics as Python's
        dict.update(...).

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
                    # TODO: reinstate with str cast: set_annotation(AO, k, str(E[k]))
                    set_annotation(AO, k, E[k])
            else:
                for k, v in E:
                    # TODO: reinstate with str cast: set_annotation(AO, k, str(v))
                    set_annotation(AO, k, v)

        for k in F:
            # TODO: reinstate with str cast: set_annotation(AO, k, str(F[k]))
            set_annotation(AO, k, F[k])

    # string annotation(string key) except+ err
    # string annotation(string key, string default) except+ err

    def setAnnotation(self, k, v):
        AO = self._AnalysisObject()
        set_annotation(AO, k, v)

    # def hasAnnotation(self, k):
    #     AO = self._AnalysisObject()
    #     return AO.hasAnnotation(string(k))

    # def rmAnnotation(self, k):
    #     AO = self._AnalysisObject()
    #     return AO.rmAnnotation(string(k))

    def clearAnnotations(self):
        AO = self._AnalysisObject()
        AO.clearAnnotations()


    def string(self):
        """
        A human readable representation of this object as it would be
        stored in a YODA file.
        """
        f = StringIO()
        writeYODA([self], f)
        f.seek(0)
        return f.read().strip()

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
