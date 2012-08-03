from string import Template
from itertools import product, izip

# A total mess. Uses python's string.Template to substitute values in various
# templated C++ wrappers, as Cython has no support for templates (and fused
# types are currently useless).

import os.path
INCLUDE_DIR = 'include'
GENERATED_DIR = os.path.join(INCLUDE_DIR, 'generated')
BASE_DIR = 'yoda'

def path(path):
    return os.path.join(BASE_DIR, path)

# Try and make the template (output) directory
try:
    os.mkdir(path(GENERATED_DIR))
except OSError:
    pass

GENERATED_HEADER = Template("""

#################################################
#############       WARNING      ################
#################################################
# This file has been automatically generated.   
# Any changes you make here will get overridden.
# Instead, make your changes in 
#               ${filename}.pyx
#################################################

""".lstrip())

def make_templates(filename, *args, **kwargs):
    individuals = args or [dict(izip(kwargs, line)) for line in 
                           product(*kwargs.itervalues())]

    full_filename = os.path.join(INCLUDE_DIR, filename + '.pyx')
    with open(path(full_filename)) as f:
        template = Template(f.read())

    plain_name = filename
    for i in individuals[0]:
        plain_name = plain_name.replace(i, '')
    plain_name = plain_name.rstrip('_')
    #print plain_name

    includes = []
    for a in individuals:

        newname = filename
        for i, j in a.iteritems(): 
            newname = newname.replace(i, j)

        out_filename = os.path.join(GENERATED_DIR, newname + '.pyx')
        with open(path(out_filename), 'w') as f:
            f.write(GENERATED_HEADER.substitute(filename=filename))
            f.write(template.substitute(**a))
        includes.append(out_filename)
        
        inc_filename = os.path.join(INCLUDE_DIR, plain_name + '.pxi')

        with open(path(inc_filename), 'w') as f:
            f.write(GENERATED_HEADER.substitute(filename=filename))
            f.write("\n".join('include "%s"' % i for i in includes))

