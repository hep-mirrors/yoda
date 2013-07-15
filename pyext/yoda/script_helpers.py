import os

def parse_x2y_args(args, xextn, yextn):
    """Helper function for working out input and output filename for YODA
    converter scripts, where the output name is optional: a default output name
    will be constructed by replacing the input file extension (xextn) with the
    output one (yextn). The x/yextn arguments should include the leading dot."""
    infile = None
    outfile = None
    if len(args) == 1:
        infile = args[0]
        if infile.endswith(xextn):
            outfile = infile.replace(xextn, yextn)
        else:
            outfile = infile + yextn
        outfile = os.path.basename(outfile)
    elif len(args) == 2:
        infile = args[0]
        outfile = args[1]
    return infile, outfile
