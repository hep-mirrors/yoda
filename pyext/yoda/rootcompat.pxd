cimport cpython
from yoda.declarations cimport Histo1D, Histo2D, Profile1D, Profile2D, Scatter2D, Scatter3D

cdef extern from "TObject.h":
    cdef cppclass TObject:
        pass

cdef extern from "TH1.h":
    cdef cppclass TH1(TObject):
        pass
    cdef cppclass TH1D(TObject):
        TH1D(TH1D)

cdef extern from "TProfile.h":
    cdef cppclass TProfile(TObject):
        TProfile(TProfile)

cdef extern from "TGraphAsymmErrors.h":
    cdef cppclass TGraphAsymmErrors(TObject):
        TGraphAsymmErrors(TGraphAsymmErrors)

cdef extern from "TH2.h":
    cdef cppclass TH2(TObject):
        pass
    cdef cppclass TH2D(TObject):
        TH2D(TH2D)


cdef extern from "pyroot_helpers.hh":
    cpython.PyObject* root_to_py_owned(TObject* root_obj)


cdef extern from "YODA/ROOTCnv.h" namespace "YODA":

    Scatter2D toScatter2D(TH1*)
    Scatter2D toScatter2D(TProfile*)
    Scatter3D toScatter3D(TH2D*)
    # Scatter3D toScatter3D(TProfile2D*)

    TH1D toTH1D(Histo1D)
    TH2D toTH2D(Histo2D)
    TProfile toTProfile(Profile1D)
    TGraphAsymmErrors toTGraph(Histo1D)
    TGraphAsymmErrors toTGraph(Profile1D)
    TGraphAsymmErrors toTGraph(Scatter2D)
    # TGraphAsymmErrors toTGraph(Histo2D)
    # TGraphAsymmErrors toTGraph(Profile2D)
    # TGraphAsymmErrors toTGraph(Scatter3D)
