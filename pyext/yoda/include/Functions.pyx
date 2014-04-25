def mkScatter(ao):
    """None -> Scatter{1,2,3}D
    Convert an AnalysisObject to a Scatter, using the logic of the bound mkScatter methods."""
    return ao.mkScatter()


def linspace(nbins, start, end):
    """(int, float, float) -> list[float]
    Make a list of n+1 bin edges linearly spaced between start and end, with the first and
    last edges on those boundaries."""
    return c.linspace(nbins, start, end)


def logspace(nbins, start, end):
    """(int, float, float) -> list[float]
    Make a list of n+1 bin edges linearly spaced on the interval log(start..end), with
    the first and last edges on those boundaries."""
    return c.logspace(nbins, start, end)


def index_between(x, binedges):
    """(float, list[float]) -> int
    Return the index of the bin which would contain x, or -1 if there is no enclosing
    bin in the given set of n+1 bin edges."""
    return c.index_between(x, binedges)


def mean(sample):
    """(list[float]) -> float
    Return the unweighted mean of the entries in the provided sample list."""
    return c.mean(sample)


def covariance(sample1, sample2):
    """(list[float], list[float]) -> float
    Return the unweighted covariance of the two provided sample lists."""
    return c.covariance(sample1, sample2)


def correlation(sample1, sample2):
    """(float, list[float]) -> int
    Return the unweighted correlation of the two provided sample lists."""
    return c.correlation(sample1, sample2)
