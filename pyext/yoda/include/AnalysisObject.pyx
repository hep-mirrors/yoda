
cdef class AnalysisObject(util.Base):
    """
    AnalysisObject is the base class of the main user-facing objects, such as
    the Histo, Profile and Scatter classes.
    """

    # Pointer upcasting mechanism
    cdef inline c.AnalysisObject *_AnalysisObject(self) except NULL:
        return <c.AnalysisObject *> self.ptr()

    # Deallocator (only needed as a base class)
    def __dealloc__(self):
        p = self._AnalysisObject()
        if self._deallocate:
            del p


    # TODO: use a proxy object so that this could also be *modified* as a dict... or hide
    @property
    def annotations(self):
        """() -> dict
        Key value pairs of metadata, returned as a Python dictionary.
        Note that this is read-only: changes to the dict will not be stored."""
        from cython.operator cimport dereference as deref, preincrement as preinc
        ana = self._AnalysisObject().annotations()
        it = ana.begin()
        out_dict = {}
        while it != ana.end():
            out_dict[deref(it).first.c_str()] = deref(it).second.c_str()
            preinc(it)
        return out_dict


    # # TODO: Yuck! Get rid of this
    # def updateAnnotations(self, E=None, **F):
    #     """
    #     AO.update([E, ]**F) -> None.  Update annotations of AO from
    #     dict/iterable E and F. Has the same semantics as Python's
    #     dict.update(...).

    #     If E present and has a .keys() method:
    #         for k in E: AO[k] = E[k]
    #     If E present and lacks .keys() method:
    #         for (k, v) in E: AO[k] = v
    #     In either case, this is followed by:
    #         for k in F: AO[k] = F[k]
    #     """
    #     AO = self._AnalysisObject()
    #     if E is not None:
    #         if hasattr(E, 'keys'):
    #             for k in E:
    #                 # TODO: reinstate with str cast: set_annotation(AO, k, str(E[k]))
    #                 set_annotation(AO, k, E[k])
    #         else:
    #             for k, v in E:
    #                 # TODO: reinstate with str cast: set_annotation(AO, k, str(v))
    #                 set_annotation(AO, k, v)

    #     for k in F:
    #         # TODO: reinstate with str cast: set_annotation(AO, k, str(F[k]))
    #         set_annotation(AO, k, F[k])


    def annotation(self, string k, default=None):
        """Get annotation k from this object (falling back to default if not set)."""
        try:
            return util._autotype(self._AnalysisObject().annotation(string(k)))
        except:
            return default

    def setAnnotation(self, string k, v):
        """Set annotation k on this object."""
        self._AnalysisObject().setAnnotation(k, util._autostr(v))

    def hasAnnotation(self, string k):
        """Check if this object has annotation k."""
        return self._AnalysisObject().hasAnnotation(string(k))

    def rmAnnotation(self, string k):
        """Remove annotation k from this object."""
        self._AnalysisObject().rmAnnotation(string(k))

    def clearAnnotations(self):
        """Clear the annotations dictionary."""
        self._AnalysisObject().clearAnnotations()


    def dump(self):
        """A human readable representation of this object."""
        from cStringIO import StringIO
        f = StringIO()
        writeFLAT([self], f)
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
        """
        Convenient access to the histogram title (optional).
        """
        def __get__(self):
            return self._AnalysisObject().title().c_str()

        def __set__(self, char *title):
            self._AnalysisObject().setTitle(string(title))


    def __repr__(self):
        return "<%s '%s'>" % (self.__class__.__name__, self.path)
