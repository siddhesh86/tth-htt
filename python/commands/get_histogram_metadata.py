from tthAnalysis.HiggsToTauTau.jobTools import run_cmd
from tthAnalysis.HiggsToTauTau.commands import *

def get_histogram_metadata(histogram):
    sha1sum = run_cmd('sha1sum %s' % histogram).split(' ')[0]
    events_count = get_events_count(histogram)

    metadata = """sha1sum: %s\nevents_count: %i\n""" % (sha1sum, events_count)
    return metadata
