Welcome to YODA's documentation!
================================

.. toctree::
   :maxdepth: 2

This is the documentation for the Python interface to the YODA data analysis
package. It's rather bare-bones at the moment, and based largely on the
docstrings embedded in the Python code. So the documentation you see here is the
same as the documentation you can get by calling `pydoc` or by using the
`help()` function in the Python interpreter, e.g.::

    $ python
    >>> import yoda
    >>> help(yoda.read)
    >>> help(yoda.Histo1D)

As time goes by we'll fill this documentation out with more examples etc. -- at
least that's the plan. Feel free to hassle the authors at
yoda@projects.hepforge.org if we don't do it, or if you find things we can
improve either in the documentation or the package itself.


Scripts
=======

TODO: the set of `yoda*` scripts needs to be documented here. For now, you can
find out how to use them by calling e.g. `yodafoo --help`. Here is the current
list of scripts that you can try: ::

    aida2flat
    aida2yoda
    flat2yoda
    yoda2aida
    yoda2flat
    yoda2root
    yodacmp
    yodacnv
    yoda-config
    yodadiff
    yodahist
    yodals
    yodamerge
    yodaplot
    yodascale


Code docs
=========

.. toctree::
   :maxdepth: 2


Basic usage
-----------

- Booking and filling a histogram called '/foo' with 20 bins between 0 and 1: ::

      import yoda, random
      h = yoda.Histo1D(20, 0.0, 1.0, "/foo")
      print h
      => <Histo1D '/foo' 20 bins, sumw=0, xmean=None>
      for _ in range(1000):
          h.fill(random.uniform(-0.5, 1.5))
      print h
      => <Histo1D '/foo' 20 bins, sumw=1e+03, xmean=4.84e-01>

- Extracting histogram statistical properties: ::

      ## Binned range extent:
      print h.xMin, h.xMax
      => 0.0 1.0

      ## Sum-of-weights / integral for whole-range and binned-range only:
      print h.sumW(), h.sumW(False)
      => 1000.0 485.0

      ## All-range and binned-range whole-histo stats:
      print h.xMean(), h.xStdDev(), h.xVariance()
      => 0.484427317353 0.581387076141 0.338010932304
      print h.xMean(False), h.xStdDev(False), h.xVariance(False)
      => 0.513167582561 0.284805437402 0.0811141371739

- Extracting bin properties: ::

      ## Looping over bins:
      for b in h.bins:
          print b
      => <HistoBin1D x=[0, 0.05)>
      => <HistoBin1D x=[0.05, 0.1)>
      => <HistoBin1D x=[0.1, 0.15)>
      => ...

      ## Extract
      for b in h.bins:
          print b.xMin, b.xMid, b.xMax, b.xFocus, b.height, b.area, b.sumW2, b.xMean, b.xStdDev
      => 0.0 0.025 0.05 0.0277156583946 380.0 19.0 19.0 0.0277156583946 0.0156189244522
      => 0.05 0.075 0.1 0.077004858171 440.0 22.0 22.0 0.077004858171 0.0160044669375
      => 0.1 0.125 0.15 0.124272408837 640.0 32.0 32.0 0.124272408837 0.01703599909
      => ...

- Saving to file, and reading back: ::

      ## Saving
      yoda.write(h, "foo.yoda") #< can pass any iterable of histos or other analysis objects

      ## Reading to a dict, indexed by histo paths
      aos = yoda.read("foo.yoda")
      print aos
      => {'/foo': <Histo1D '/foo' 20 bins, sumw=1e+03, xmean=4.84e-01>}

      ## Reading to a list
      aos = yoda.read("foo.yoda", asdict=False)
      print aos
      => [<Histo1D '/foo' 20 bins, sumw=1e+03, xmean=4.84e-01>]
      print aos[0].path
      => /foo

- Converting to a Scatter, and looping over points: ::

      s = h.mkScatter()  # <- you may also read scatters from file, e.g. reference data
      print s
      => <Scatter2D '/foo' 20 points>

      for p in s.points:
          print p.x, p.y, p.xErrs, p.yErrAvg, p.errAvg(2)
      => 0.025 380.0 ErrorPair(minus=0.025, plus=0.025) 87.1779788708 87.1779788708
      => 0.075 440.0 ErrorPair(minus=0.02500000000000001, plus=0.024999999999999994) 93.8083151965 93.8083151965
      => ...

- Analysis object metadata: ::

      print s.annotations
      => ['Path', 'Title', 'Type']
      s.setAnnotation("Foo", "My own private Patrick Swayze")
      print s.annotations
      => ['Foo', 'Path', 'Title', 'Type']
      print s.annotation("Foo")
      => 'My own private Patrick Swayze'


Tips and tricks
---------------

- Filtering analysis objects by path: ::

      aos = yoda.read("mydata.yoda")
      hs = [h for h in aos.values() if "foo" in h.path]

  or, if you have YODA version >= 1.3.2, you can do regex filtering inline
  during analysis object reading from file: ::

      aos = yoda.read("mydata.yoda", ".*foo.*")

- Summing histograms: ::

      import operator
      hsum = reduce(operator.add, hs)


Bin/point path searching
------------------------

The `yoda.search` subpackage provides tools for finding histogram bins and
scatter points based on their parent objects' paths, and on the bin/point index
or the physical location of bin edges.

.. automodule:: yoda.search
   :members:


Histogram plotting
------------------

The `yoda.plotting` module provides in-development tools for conveniently
comparing and plotting YODA objects using matplotlib_.

.. automodule:: yoda.plotting
   :members:

.. _matplotlib: http://www.matplotlib.org/


Miscellaneous
-------------

Other functionality in the `yoda` package...

.. automodule:: yoda.script_helpers
   :members:


Main YODA objects
-----------------

The wrappers on the core C++ YODA data types and functions are all defined in
the `yoda.core` module, which is generated semi-automatically by the Cython
system. However, they are all available directly in the `yoda` namespace by
virtue of a `from yoda.core import *` in the `yoda` package.

.. automodule:: yoda.core
   :members:


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
