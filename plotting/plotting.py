import yoda
import numpy as np


def as_bool(x):
    if type(x) is bool:
        return x
    s = str(x)
    if x.lower() in ("true", "yes", "on", "1"): return True
    if x.lower() in ("false", "no", "off", "0"): return False
    raise Exception("'{}' cannot be parsed as a boolean flag".format(s))


class NumpyHist(object):
    def __init__(self, ao):
        if not isinstance(ao, yoda.AnalysisObject):
            raise Exception("ao argument must be a YODA AnalysisObject")
        ## Get annotations
        self.path = ao.path
        self.annotations = {aname : ao.annotation(aname) for aname in ao.annotations}
        ## Get data points
        points = ao.points if type(ao) is yoda.Scatter2D else ao.mkScatter().points
        self.data = np.zeros(len(points), dtype={'names':['x', 'y', 'exminus', 'explus', 'eyminus', 'eyplus'],
                                                 'formats':['f4', 'f4', 'f4', 'f4', 'f4', 'f4']}).view(np.recarray)
        for i, p in enumerate(points):
            self.data.x[i] = p.x
            self.data.y[i] = p.y
            self.data.exminus[i] = p.xErrs[0]
            self.data.explus[i]  = p.xErrs[1]
            self.data.eyminus[i] = p.yErrs[0]
            self.data.eyplus[i]  = p.yErrs[1]


    def __len__(self):
        return len(self.x)


    @property
    def xedges_sgl(self):
        return np.append(self.xmin, self.xmax[-1])

    @property
    def xedges_dbl(self):
        edges = np.empty((2*len(self.x),), dtype=self.x.dtype)
        edges[0::2] = self.xmin
        edges[1::2] = self.xmax
        return edges


    @property
    def xmin(self):
        return self.x - self.exminus

    @property
    def xmax(self):
        return self.x + self.explus


    @property
    def ymin(self):
        return self.y - self.eyminus

    @property
    def ymax(self):
        return self.y + self.eyplus


    @property
    def x(self):
        return self.data.x

    @property
    def y(self):
        return self.data.y

    @property
    def exminus(self):
        return self.data.exminus

    @property
    def explus(self):
        return self.data.explus

    @property
    def eyminus(self):
        return self.data.eyminus

    @property
    def eyplus(self):
        return self.data.eyplus

    # def __getattr__(self, attr):
    #     "Fall back to the data array for attributes not defined on NumpyHist"
    #     return getattr(self.data, attr)


    def same_binning_as(self, other):
        return (other.x == self.x).all() and \
               (other.exminus == self.exminus).all() and \
               (other.explus == self.explus).all()
