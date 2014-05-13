
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


    @property
    def type(self):
        return self._AnalysisObject().type()


    @property
    def annotations(self):
        """() -> list[str]
        A list of all annotation/metadata keys."""
        return self._AnalysisObject().annotations()

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
