cimport rootcompat as croot
import yoda
cimport yoda.declarations as cyoda
cimport yoda.util
import ROOT
cimport cython.operator.dereference as deref
#ROOT.PyConfig.IgnoreCommandLineOptions = True


# TODO: We should find a way to restructure the YODA bindings to allow external Cython
#   programs to get the used structs -- this requires using pxd files accordingly.

#def convert_yoda():
    #cdef yoda.util.Base h = yoda.Histo1D(10, 0, 1, '/test')
    #cdef cyoda.Histo1D* hptr = <cyoda.Histo1D*> h.ptr()
    #cdef TObject* thist = new TH1D(toTH1D(hptr[0]))
    #return <object> (croot.root_to_py_owned(thist))

#def toScatter2D(TH1*):


# Dispatch on python typename

def to_root(yoda.util.Base yoda_obj, asgraph=False):
    cdef void* ptr = yoda_obj.ptr()
    if isinstance(yoda_obj, yoda.Histo1D):
        return _H1toTGraph(<cyoda.Histo1D*> ptr) if asgraph else _H1toTH1D(<cyoda.Histo1D*> ptr)
    elif isinstance(yoda_obj, yoda.Profile1D):
        return _P1toTGraph(<cyoda.Profile1D*> ptr) if asgraph else _P1toTProfile(<cyoda.Profile1D*> ptr)
    elif isinstance(yoda_obj, yoda.Histo2D):
        #return _H2toTGraph(<cyoda.Histo2D*> ptr) if asgraph else _H2toTH2D(<cyoda.Histo2D*> ptr)
        return _H2toTH2D(<cyoda.Histo2D*> ptr)
    # elif isinstance(yoda_obj, yoda.Profile2D):
    #     return _P2toTGraph(<cyoda.Profile2D*> ptr) if asgraph else _P2toTProfile2D(<cyoda.Profile2D*> ptr)
    elif isinstance(yoda_obj, yoda.Scatter2D):
        return _S2toTGraph(<cyoda.Scatter2D*> ptr)
    # elif isinstance(yoda_obj, yoda.Scatter3D):
    #     return _S3toTGraph(<cyoda.Scatter3D*> ptr)


cdef object root_to_py(croot.TObject* tobj):
    return <object> croot.root_to_py_owned(tobj)


cdef _H1toTH1D(cyoda.Histo1D* h1d):
    return ROOT.TH1D(root_to_py(new croot.TH1D(croot.toTH1D(deref(h1d)))))

cdef _P1toTProfile(cyoda.Profile1D* p1d):
    return ROOT.TProfile(root_to_py(new croot.TProfile(croot.toTProfile(deref(p1d)))))

cdef _H2toTH2D(cyoda.Histo2D* h2d):
    return ROOT.TH2D(root_to_py(new croot.TH2D(croot.toTH2D(deref(h2d)))))

# cdef _P2toTProfile2D(cyoda.Profile2D* p2d):
#     return ROOT.TProfile2D(root_to_py(new croot.TProfile2D(croot.toTProfile2D(deref(p2d)))))


cdef _S2toTGraph(cyoda.Scatter2D* s2d):
    return ROOT.TGraphAsymmErrors(root_to_py(new croot.TGraphAsymmErrors(croot.toTGraph(deref(s2d)))))

cdef _H1toTGraph(cyoda.Histo1D* h1d):
    return ROOT.TGraphAsymmErrors(root_to_py(new croot.TGraphAsymmErrors(croot.toTGraph(deref(h1d)))))

cdef _P1toTGraph(cyoda.Profile1D* p1d):
    return ROOT.TGraphAsymmErrors(root_to_py(new croot.TGraphAsymmErrors(croot.toTGraph(deref(p1d)))))


# cdef _S3toTGraph(cyoda.Scatter3D* s3d):
#     return ROOT.TGraph2D(root_to_py(new croot.TGraph2D(croot.toTGraph(deref(s3d)))))

# cdef _H2toTGraph(cyoda.Histo2D* h2d):
#     return ROOT.TGraph2D(root_to_py(new croot.TGraph2D(croot.toTGraph(deref(h2d)))))

# cdef _P2toTGraph(cyoda.Profile2D* p2d):
#     return ROOT.TGraph2D(root_to_py(new croot.TGraph2D(croot.toTGraph(deref(p2d)))))
