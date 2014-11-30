import yoda
import numpy as np


## Utils

def as_bool(x):
    if type(x) is bool:
        return x
    s = str(x)
    if x.lower() in ("true", "yes", "on", "1"): return True
    if x.lower() in ("false", "no", "off", "0"): return False
    raise Exception("'{}' cannot be parsed as a boolean flag".format(s))

def autotype(var):
    """Automatically convert strings to numerical types if possible."""
    if type(var) is not str:
        return var
    if var.isdigit() or (var.startswith("-") and var[1:].isdigit()):
        return int(var)
    try:
        f = float(var)
        return f
    except ValueError:
        return var

def autostr(var, precision=8):
    """Automatically format numerical types as the right sort of string."""
    if type(var) is float:
        return ("% ." + str(precision) + "e") % var
    elif not hasattr(var, "__iter__"):
        return str(var)
    else:
        return ",".join(_autostr(subval) for subval in var)


########################


class NumpyHist(object):
    def __init__(self, ao):
        if not isinstance(ao, yoda.AnalysisObject):
            raise Exception("ao argument must be a YODA AnalysisObject")
        ## Get annotations
        self.path = ao.path
        self.annotations = {aname : ao.annotation(aname) for aname in ao.annotations}
        ## Convert to Scatter and set dimensionality & recarray column names
        s = ao.mkScatter()
        names = ['x', 'y', 'exminus', 'explus', 'eyminus', 'eyplus']
        if type(s) is yoda.Scatter2D:
            self.dim = 2
        elif type(s) is yoda.Scatter3D:
            self.dim = 3
            names.insert(2, "z")
            names += ['ezminus', 'ezplus']
        else:
            raise RuntimeError("Whoa! If ao doesn't convert to a 2D or 3D scatter, what is it?!")
        ## Put data points into numpy structure
        dtype = {"names": names, "formats": ["f4" for _ in names]}
        self.data = np.zeros(len(s.points), dtype).view(np.recarray)
        for i, p in enumerate(s.points):
            self.data.x[i] = p.x
            self.data.exminus[i] = p.xErrs[0]
            self.data.explus[i]  = p.xErrs[1]
            self.data.y[i] = p.y
            self.data.eyminus[i] = p.yErrs[0]
            self.data.eyplus[i]  = p.yErrs[1]
            if self.dim > 2:
                self.data.z[i] = p.z
                self.data.ezminus[i] = p.zErrs[0]
                self.data.ezplus[i]  = p.zErrs[1]


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


    # TODO: automate more with __get/setattr__?

    @property
    def x(self):
        return self.data.x

    @property
    def exminus(self):
        return self.data.exminus

    @property
    def explus(self):
        return self.data.explus


    @property
    def y(self):
        return self.data.y

    @property
    def eyminus(self):
        return self.data.eyminus

    @property
    def eyplus(self):
        return self.data.eyplus


    # TODO: don't provide these / throw helpful errors if only 2D

    @property
    def z(self):
        return self.data.z

    @property
    def ezminus(self):
        return self.data.ezminus

    @property
    def ezplus(self):
        return self.data.ezplus


    # def __getattr__(self, attr):
    #     "Fall back to the data array for attributes not defined on NumpyHist"
    #     return getattr(self.data, attr)


    def same_binning_as(self, other):
        if self.dim != other.dim:
            return False
        if not (other.x == self.x).all() and \
               (other.exminus == self.exminus).all() and \
               (other.explus == self.explus).all():
            return False
        if self.dim == 2:
            return True
        return (other.y == self.y).all() and \
               (other.eyminus == self.eyminus).all() and \
               (other.eyplus == self.eyplus).all()


##########

def read_plot_keys(datfile):
    import re
    re_begin = re.compile("#*\s*BEGIN\s+PLOT\s*(\w*)")
    re_comment = re.compile("#.*")
    re_attr = re.compile("(\w+)\s*=\s*(.*)")
    re_end = re.compile("#*\s*END\s+PLOT\s+\w*")
    plotkeys = {}
    with open(datfile) as f:
        inplot = False
        name = None
        for line in f:
            l = line.strip()
            if re_begin.match(l):
                inplot = True
                name = re_begin.match(l).group(1)
            elif re_end.match(l):
                inplot = False
                name = None
            elif re_comment.match(l):
                continue
            elif inplot:
                m = re_attr.match(l)
                if m is None: continue
                plotkeys.setdefault(name, {})[m.group(1)] = m.group(2)
    return plotkeys


## Plotting helper functions

import matplotlib as mpl

def mk_figaxes(ratio=True, title=None, figsize=(8,6)):
    "Make figure and subplot grid layout"
    from matplotlib import pyplot as plt
    fig = plt.figure(figsize=figsize)
    if title:
        fig.suptitle(title, x=0.0)
    #
    if ratio:
        gs = mpl.gridspec.GridSpec(2, 1, height_ratios=[3,1], hspace=0)
    else:
        gs = mpl.gridspec.GridSpec(1, 1, hspace=0)
    axmain = fig.add_subplot(gs[0])
    axmain.hold(True)
    #
    try:
        axratio = fig.add_subplot(gs[1], sharex=axmain)
        axratio.hold(True)
        axratio.axhline(1.0, color="gray") #< Ratio = 1 marker line
    except IndexError, e:
        axratio = None
    #
    return fig, axmain, axratio


def set_axis_labels(axmain, axratio, xlabel=None, ylabel=None, ratioylabel=None):
    axmain.set_ylabel(ylabel, y=1, ha="right", labelpad=None)
    if axratio:
        axmain.xaxis.set_major_locator(mpl.ticker.NullLocator())
        axratio.set_xlabel(xlabel, x=1, ha="right", labelpad=None)
        axratio.set_ylabel(ratioylabel)
    else:
        axmain.set_xlabel(xlabel, x=1, ha="right", labelpad=None)


def setup_axes(axmain, axratio, plotkeys):
    ## Axis labels first
    xlabel = plotkeys.get("XLabel", "")
    ylabel = plotkeys.get("YLabel", "")
    ratioylabel = plotkeys.get("RatioYLabel", "Ratio")
    set_axis_labels(axmain, axratio, xlabel, ylabel, ratioylabel)

    ## log/lin measures
    # TODO: Dynamic default based on data ranges?
    # TODO: take log axes and preference for round numbers into account in setting default axis limits
    xmeasure = "log" if as_bool(plotkeys.get("LogX", False)) else "linear"
    ymeasure = "log" if as_bool(plotkeys.get("LogY", False)) else "linear"
    ratioymeasure = "log" if as_bool(plotkeys.get("RatioLogY", False)) else "linear"
    axmain.set_xscale(xmeasure)
    axmain.set_yscale(ymeasure)
    if axratio:
        axratio.set_xscale(xmeasure)
        axratio.set_yscale(ratioymeasure)

    ## Plot range limits
    if plotkeys.has_key("YMin"):
        axmain.set_ylim(bottom=float(plotkeys.get("YMin")))
    if plotkeys.has_key("YMax"):
        axmain.set_ylim(top=float(plotkeys.get("YMin")))
    #
    if plotkeys.has_key("XMin"):
        axmain.set_xlim(left=float(plotkeys.get("XMin")))
    if plotkeys.has_key("XMax"):
        axmain.set_xlim(right=float(plotkeys.get("XMin")))
    #
    if axratio:
        # TODO: RatioSymmRange option
        # axratio.set_xlim([xmin-0.001*xdiff, xmax+0.001*xdiff]) # <- TODO: bad on a log scale!
        if plotkeys.has_key("XMin"):
            axratio.set_xlim(left=float(plotkeys.get("XMin")))
        if plotkeys.has_key("XMax"):
            axratio.set_xlim(right=float(plotkeys.get("XMin")))
        if plotkeys.has_key("RatioYMin"):
            axratio.set_ylim(bottom=float(plotkeys.get("RatioYMin")))
        if plotkeys.has_key("RatioYMax"):
            axratio.set_ylim(top=float(plotkeys.get("RatioYMax")))

    # TODO: Ratio plot manual ticks
