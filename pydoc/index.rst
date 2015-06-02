.. YODA documentation master file, created by
   sphinx-quickstart on Wed Oct 29 21:06:27 2014.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

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


Main YODA objects
-----------------

The wrappers on the core C++ YODA data types and functions are all defined in
the `yoda.core` module, which is generated semi-automatically by the Cython
system. However, they are all available directly in the `yoda` namespace by
virtue of a `from yoda.core import *` in the `yoda` package.

.. automodule:: yoda.core
   :members:


Tips and tricks
---------------

- Filtering analysis objects by path::

      hs = [h for h in aos.values() if "foo" in h.path]

- Summing histograms::

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



Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
