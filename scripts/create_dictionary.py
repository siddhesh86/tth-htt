#!/usr/bin/env python
import argparse, os.path, sys, logging, imp, jinja2, ROOT, re, ctypes, copy

try:
    from urllib.parse import urlparse
except ImportError:
     from urlparse import urlparse

class hdfs:
  class hdfsFileInfo(ctypes.Structure):
    _fields_ = [
      ('mKind',        ctypes.c_int),
      ('mName',        ctypes.c_char_p),
      ('mLastMod',     ctypes.c_int64),
      ('mSize',        ctypes.c_int64),
      ('mReplication', ctypes.c_short),
      ('mBlockSize',   ctypes.c_int64),
      ('mOwner',       ctypes.c_char_p),
      ('mGroup',       ctypes.c_char_p),
      ('mPermissions', ctypes.c_int32),
      ('mLastAccess',  ctypes.c_int64),
    ]

  class info:
    def __init__(self, hdfsFileInfoObject):
      self.kind          = chr(hdfsFileInfoObject.contents.mKind)
      self.name          = hdfs.normalize_path(hdfsFileInfoObject.contents.mName)
      self.size          = hdfsFileInfoObject.contents.mSize
      self.name_fuse     = hdfs.fuse_path(self.name)
      self.basename      = os.path.basename(self.name_fuse)
      self.depth         = len(self.name_fuse.split(os.path.sep)) - 1
      self.sparent_depth = -1

    def __str__(self):
      return self.name_fuse

    def __repr__(self):
      return self.name

    def is_file(self):
      return self.kind == 'F'

    def is_dir(self):
      return self.kind == 'D'

  class hdfsException(Exception):
    pass

  def __init__(self):
    self.lib_path = "/usr/lib64/libhdfs.so"
    if not os.path.isfile(self.lib_path):
      raise hdfs.hdfsException("No such file: %s" % self.lib_path)

    logging.debug("Loading {lib}".format(lib = self.lib_path))
    self.lib = ctypes.cdll.LoadLibrary(self.lib_path)
    self.lib.hdfsListDirectory.restype = ctypes.POINTER(hdfs.hdfsFileInfo)
    self.lib.hdfsGetPathInfo.restype   = ctypes.POINTER(hdfs.hdfsFileInfo)
    self.hdfsFileInfo_size             = ctypes.sizeof(hdfs.hdfsFileInfo)

    logging.debug("Building HDFS interface")
    self.bld = self.lib.hdfsNewBuilder()
    if not self.bld:
      raise hdfs.hdfsException("Could not create new HDFS interface")
    self.lib.hdfsBuilderSetNameNode(self.bld, "default")
    self.lib.hdfsBuilderSetNameNodePort(self.bld, 0)

    logging.debug("Connecting to the HDFS interface")
    self.fs = self.lib.hdfsBuilderConnect(self.bld)
    if not self.fs:
      raise hdfs.hdfsException("Could not connect to the HDFS interface")

  @staticmethod
  def normalize_path(path):
    if not path.startswith('/hdfs') and not path.startswith("hdfs://"):
      raise hdfs.hdfsException("Invalid path: %s" % path)
    path_t = re.sub('^/hdfs', 'hdfs://', path) if path.startswith('/hdfs') else path
    path_url = urlparse(path_t)
    path_f = path_url.scheme + '://' + path_url.path
    return path_f

  @staticmethod
  def fuse_path(path):
    if not path.startswith("hdfs://"):
      raise hdfs.hdfsException("Invalid path: %s" % path)
    return re.sub('^hdfs://', '/hdfs', path)

  def get_path_info(self, path):
    normalized_path = hdfs.normalize_path(path)
    path_info = self.lib.hdfsGetPathInfo(self.fs, normalized_path)
    if not path_info:
      raise hdfs.hdfsException("No such path: %s" % normalized_path)
    path_obj = hdfs.info(path_info)
    self.lib.hdfsFreeFileInfo(path_info, 1)
    return path_obj

  def get_dir_entries(self, path_obj):
    nof_entries = ctypes.c_int()
    dir_ptr = self.lib.hdfsListDirectory(self.fs, path_obj.name, ctypes.byref(nof_entries))
    if not dir_ptr:
      raise hdfs.hdfsException("No such path: %s" % path_obj.name)
    dir_ptr_copy = dir_ptr
    entries = []
    for j in range(nof_entries.value):
      entries.append(hdfs.info(dir_ptr))
      void_p  = ctypes.cast(dir_ptr, ctypes.c_voidp).value + self.hdfsFileInfo_size
      dir_ptr = ctypes.cast(void_p, ctypes.POINTER(hdfs.hdfsFileInfo))
    self.lib.hdfsFreeFileInfo(dir_ptr_copy, nof_entries)
    return entries

class FileTracker:
  def __init__(self):
    self.zero_file_size  = []
    self.zombie_files    = []
    self.corrupted_files = []

def load_dict(path, name):
  if not os.path.isfile(path):
    logging.error("No such dictionary file: {dict_path}".format(dict_path = path))
    sys.exit(1)
  imp_dict = imp.load_source('', path)
  if not hasattr(imp_dict, name):
    logging.error("No such dictionary in the file '{dict_path}': {dict_name}".format(
      dict_path = path, dict_name = name,
    ))
    sys.exit(1)
  samples = getattr(imp_dict, name)
  return samples

header_str = """from collections import OrderedDict as OD

# file generated with the following command:
# {{ command }}

{{ dict_name }} = OD()

"""

dictionary_entry_str = """{{ dict_name }}["{{ dbs_name }}"] = OD([
  ("type",                            "{{ sample_type }}"),
  ("sample_category",                 "{{ sample_category }}"),
  ("process_name_specific",           "{{ process_name_specific }}"),
  ("nof_files",                       {{ nof_files }}),
  ("nof_events",                      {{ nof_events }}),
  ("use_HIP_mitigation_bTag",         {{ use_HIP_mitigation_bTag }}),
  ("use_HIP_mitigation_mediumMuonId", {{ use_HIP_mitigation_mediumMuonId }}),
  ("use_it",                          {{ use_it }}),{% if sample_type == "mc" %}
  ("xsection",                        {{ xsection }}),
  ("genWeight",                       {{ genWeight }}),{% endif %}
  ("triggers",                        {{ triggers }}),
  ("reHLT",                           {{ reHLT }}),
  ("local_paths",
    [
{{ paths }}
    ]
  ),
])
"""

path_entry_str = """      OD([
        ("path",      "{{ path }}"),
        ("selection", "{{ selection }}"),
        ("blacklist", {{ blacklist }}),
      ]),
"""

class PathEntry:
  def __init__(self, path, nof_files, blacklist, nof_events):
    self.path       = path
    self.nof_files  = nof_files
    self.blacklist  = blacklist
    self.nof_events = nof_events
    self.selection  = [] # if empty, select all

  def __repr__(self):
    return self.path

def get_triggers(process_name_specific, is_data):
  if 'SingleElec' in process_name_specific:
    return ['1e']
  if 'SingleMuon' in process_name_specific:
    return ['1mu']
  if 'DoubleEG' in process_name_specific:
    return ['2e', '3e']
  if 'DoubleMuon' in process_name_specific:
    return ['2mu', '3mu']
  if 'MuonEG' in process_name_specific:
    return ['1e1mu', '2e1mu', '1e2mu']
  if 'Tau' in process_name_specific:
    return ['1e1tau', '1mu1tau', '2tau']
  if is_data:
    raise ValueError("Expected MC!")
  return [
    '1e', '1mu', '2e', '2mu', '1e1mu', '3e', '3mu', '2e1mu', '1e2mu', '1e1tau', '1mu1tau', '2tau'
  ]

def process_paths(meta_dict, key):
  local_paths = meta_dict[key]['paths']
  if len(local_paths) == 1:
    meta_dict[key]['nof_files'] = local_paths[0].nof_files
    meta_dict[key]['nof_events'] = int(local_paths[0].nof_events)
    meta_dict[key]['local_paths'] = [{
      'path'      : local_paths[0].path,
      'selection' : '*',
      'blacklist' : local_paths[0].blacklist,
    }]
  elif len(local_paths) > 1:
    max_nof_files = max([path_entry.nof_files for path_entry in local_paths])
    sum_of_events = int(sum([path_entry.nof_events for path_entry in local_paths]))
    meta_dict[key]['nof_files'] = max_nof_files
    meta_dict[key]['nof_events'] = sum_of_events
    meta_dict[key]['local_paths'] = []

    # sort the paths by the largest coverage
    local_paths_sorted = list(sorted(
      local_paths,
      key = lambda local_path: local_path.nof_files - len(local_path.blacklist),
      reverse = True,
    ))
    # determine which files to select in secondary storages
    for blacklisted_idx in local_paths_sorted[0].blacklist:
      for local_path in local_paths_sorted[1:]:
        if blacklisted_idx not in local_path.blacklist and blacklisted_idx < local_path.nof_files:
          local_path.selection.append(blacklisted_idx)
    for local_path in local_paths_sorted:
      meta_dict[key]['local_paths'].append({
        'path'      : local_path.path,
        'selection' : '*' if not local_path.selection else ",".join(map(str, local_path.selection)),
        'blacklist' : local_path.blacklist,
      })
  else:
    raise ValueError("Not enough paths to locate for %s" % key)

def traverse_single(hdfs_system, meta_dict, path_obj, key, histogram_name, check_every_event,
                    filetracker, file_idx):
  ''' Assume that the following subdirectories are of the form: 0000, 0001, 0002, ...
      In these directories we expect root files of the form: tree_1.root, tree_2.root, ...
      If either of those assumptions doesn't hold, we bail out; no clever event count needed
  :param hdfs_system:       The HDFS interface (instance of hdfs)
  :param meta_dict:         Meta-dictionary
  :param path_obj:          Contains meta-information about a path (instance of hdfs.info)
  :param key:               Key to the meta-dictionary the entry of which will be updated
  :param histogram_name:    Name of the histogram containing event counts
  :param check_every_event: Name of the TTree to be looped over for error checking purposes
  :param filetracker:       An instance of FileTracker() for logging broken files
  :param file_idx:          Index of the corrupted file
  :return: None
  '''
  if 'paths' not in meta_dict[key]:
    meta_dict[key]['paths'] = []
  if path_obj.name_fuse in meta_dict[key]['paths']:
    logging.warning("Path {path} has already been traversed".format(path = path_obj.name_fuse))
    return

  logging.info("Single-traversing {path}".format(path = path_obj.name))
  entries = hdfs_system.get_dir_entries(path_obj)
  entries_valid = []
  for entry in entries:
    if not entry.is_dir():
      continue
    if len(entry.basename) != 4:
      continue
    try:
      int(entry.basename)
    except:
      continue
    entries_valid.append(entry)

  digit_regex = re.compile(r"tree_(?P<i>\d+)\.root")
  is_data = meta_dict[key]['sample_category'] == 'data_obs'
  histogram_name_t = histogram_name if not is_data else 'Count'
  nof_events = 0.

  for entry in entries_valid:
    subentries = hdfs_system.get_dir_entries(entry)
    subentry_files = filter(lambda path: path.is_file(), subentries)
    indices = []
    for subentry_file in subentry_files:
      digit_match = digit_regex.search(subentry_file.basename)
      if not digit_match:
        continue
      matched_idx = int(digit_match.group('i'))
      if file_idx > 0 and matched_idx != file_idx:
        logging.debug("Skipping file {path}".format(path = subentry_file.name))
        continue

      if subentry_file.size == 0:
        logging.debug("File {path} has a file size of 0".format(path = subentry_file.name))
        filetracker.zero_file_size.append(subentry_file.name)
        continue
      logging.debug("Opening file {path}".format(path = subentry_file.name))
      f = ROOT.TFile.Open(subentry_file.name, "read")
      if not f:
        raise ValueError("Could not open {path}".format(path = subentry_file.name))
      if f.IsZombie():
        logging.warning("File {path} is a zombie".format(path = subentry_file.name))
        f.Close()
        del f
        filetracker.zombie_files.append(subentry_file.name)
        continue
      if check_every_event:
        logging.info("Inspecting file {path} for corruption".format(path = subentry_file.name))
        if check_every_event not in f.GetListOfKeys():
          raise ValueError("Tree of the name {tree} is not in file {path}".format(
            tree = check_every_event,
            path = subentry_file.name,
          ))
        t = f.Get(check_every_event)
        if not t:
          raise ValueError("Could not find tree of the name {tree} in file {path}".format(
            tree = check_every_event,
            path = subentry_file.name,
          ))
        nof_entries = t.GetEntries()
        for i in range(0, nof_entries):
          nof_bytes_read = t.GetEntry(i)
          if nof_bytes_read < 0:
            filetracker.corrupted_files.append(subentry_file.name)
            logging.debug("File {path} seems to be corrupted starting from event {idx}".format(
              path = subentry_file.name,
              idx  = i,
            ))
            break
        f.Close()
        del t
        del f
        continue
      if histogram_name_t not in f.GetListOfKeys():
        raise ValueError("Histogram of the name {histogram_name} is not in file {path}".format(
          histogram_name = histogram_name_t,
          path           = subentry_file.name,
        ))
      h = f.Get(histogram_name_t)
      if not h:
        raise ValueError("Could not find histogram of the name {histogram_name} in file {path}".format(
          histogram_name = histogram_name_t,
          path           = subentry_file.name,
        ))
      nof_events += h.Integral()
      f.Close()
      del h
      del f
      indices.append(matched_idx)

  if not indices:
    logging.debug("Path {path} contains no ROOT files".format(path = path_obj.name))
    return
  minimum_idx      = min(indices)
  maximum_idx      = max(indices)
  nof_files        = len(indices)
  actual_nof_files = maximum_idx if maximum_idx > nof_files else nof_files
  if minimum_idx == 0:
    logging.warning("The entry {key} has the first file starting with index 0")
  blacklist = list(sorted(list(set(range(1, actual_nof_files + 1)) - set(indices))))

  logging.debug("Found {nof_events} events in {nof_files} files in {path} for entry "
                "{key} (from which the following indices were missing: {blacklist})".format(
    nof_events = nof_events,
    nof_files  = actual_nof_files,
    path       = path_obj.name_fuse,
    key        = key,
    blacklist  = ', '.join(map(str, blacklist)) if blacklist else 'None',
  ))

  if not meta_dict[key]['located']:
    hip_mitigation_regex = re.compile(".*2016[BCDEF].*")
    meta_dict[key]['use_HIP_mitigation_bTag'] = bool(hip_mitigation_regex.match(
      meta_dict[key]['process_name_specific'])
    ) and is_data
    meta_dict[key]['use_HIP_mitigation_mediumMuonId'] = meta_dict[key]['use_HIP_mitigation_bTag']
    meta_dict[key]['triggers']                        = get_triggers(
      meta_dict[key]['process_name_specific'], is_data
    )
    meta_dict[key]['genWeight']                       = not is_data
    meta_dict[key]['type']                            = 'data' if is_data else 'mc'
    meta_dict[key]['reHLT']                           = True
    meta_dict[key]['located']                         = True
  meta_dict[key]['paths'].append(
    PathEntry(path_obj.name_fuse, actual_nof_files, blacklist, nof_events)
  )

  return

def traverse_double(hdfs_system, meta_dict, path_obj, key, histogram_name, check_every_event,
                    filetracker, file_idx):
  ''' Assume that the name of the following subdirectories are the CRAB job IDs
      The tree structure inside those directories should be the same as described in
      traverse_single()
      Therefore, we loop over the CRAB job IDs and pass each subfolder to traverse_single()
  :param hdfs_system:       The HDFS interface (instance of hdfs)
  :param meta_dict:         Meta-dictionary
  :param path_obj:          Contains meta-information about a path (instance of hdfs.info)
  :param key:               Key to the meta-dictionary the entry of which will be updated
  :param histogram_name:    Name of the histogram containing event counts
  :param check_every_event: Name of the TTree to be looped over for error checking purposes
  :param filetracker:       An instance of FileTracker() for logging broken files
  :param file_idx:          Index of the corrupted file
  :return: None
  '''
  logging.info("Double-traversing {path}".format(path = path_obj.name))
  entries = hdfs_system.get_dir_entries(path_obj)
  for entry in entries:
    traverse_single(
      hdfs_system, meta_dict, entry, key, histogram_name, check_every_event, filetracker, file_idx
    )
  return

def obtain_paths(hdfs_system, input_path):
  paths = []
  if len(input_path):
    # check if the input path is a path or a file
    path = input_path[0]
    if path.startswith('/hdfs'):
      path_obj = hdfs_system.get_path_info(path)
      if path_obj.is_file():
        raise ValueError("Thou shalt not your list of input paths on /hdfs! Store it somewhere else!")
      else:
        paths = input_path
    else:
      if not os.path.exists(path):
        raise ValueError("No such file: {path}".format(path = path))
      if not os.path.isfile(path):
        raise ValueError("Not a file: {path}".format(path = path))
      with open(path, 'r') as f:
        for line in f:
          line_stripped = line.rstrip('\n').rstrip(os.path.sep)
          if line_stripped:
            paths.append(line_stripped)
  else:
    paths = input_path
  return paths

if __name__ == '__main__':
  logging.basicConfig(
    stream = sys.stdout,
    level  = logging.INFO,
    format = '%(asctime)s - %(levelname)s: %(message)s'
  )

  class SmartFormatter(argparse.HelpFormatter):
    def _split_lines(self, text, width):
      if text.startswith('R|'):
        return text[2:].splitlines()
      return argparse.HelpFormatter._split_lines(self, text, width)

  CORRUPTED_FILE = 'corrupted.txt'

  parser = argparse.ArgumentParser(
    formatter_class = lambda prog: SmartFormatter(prog, max_help_position = 35))
  parser.add_argument('-m', '--meta-dictionary', dest = 'meta_dictionary', metavar = 'file',
                      required = True, type = str,
                      help = 'R|Path to the meta dictionary')
  parser.add_argument('-N', '--output-dict-name', dest = 'output_dict_name', metavar = 'name',
                      type = str, default = 'sample',
                      help = 'R|Name of the output dictionary')
  parser.add_argument('-p', '--path', dest = 'path', metavar = 'directory', required = True,
                      type = str, nargs = '+',
                      help = 'R|List of full paths to the directories to scan')
  parser.add_argument('-e', '--exclude-path', dest = 'exclude_path', metavar = 'directory',
                      required = False, type = str, nargs = '+', default = [])
  parser.add_argument('-d', '--depth', dest = 'depth', metavar = 'number', required = False,
                      type = int, default = -1,
                      help = 'R|How many directory levels to traverse (default: all)')
  parser.add_argument('-f', '--filter', dest = 'filter', metavar = 'name', required = False,
                      type = str, default = '.*',
                      help = 'R|Regular expression for selecting only specific samples')
  parser.add_argument('-H', '--histogram', dest = 'histogram', metavar = 'name', type = str,
                      default = 'CountWeighted',
                      help = 'R|Expected TH1F name')
  parser.add_argument('-o', '--output-directory', dest = 'output_directory', metavar = 'path',
                      type = str, default = '.',
                      help = 'R|Output directory')
  parser.add_argument('-g', '--generate-python', dest = 'generate_python', metavar = 'name',
                      type = str, default = 'dict.py',
                      help = 'R|File name of the new python dictionary')
  parser.add_argument('-s', '--skip-header', dest = 'skip_header', action = 'store_true',
                      default = False,
                      help = 'R|Skip dictionary definitions in the output')
  parser.add_argument('-J', '--generate-jobs', dest = 'generate_jobs', metavar = 'generate_jobs',
                      type = str, default = '', required = False,
                      help = 'R|Generate SLURM jobs instead of running locally')
  parser.add_argument('-c', '--check-every-event', dest = 'check_every_event', metavar = 'name',
                      type = str, default = "", required = False,
                      help = 'R|Supply TTree name to check every event (NB! Extremely slow!)')
  parser.add_argument('-z', '--save-zombies', dest = 'save_zombies', metavar = 'save_zombies',
                      type = str, default = '',
                      help = 'R|Save the list of zombie files')
  parser.add_argument('-Z', '--save-zeroes', dest = 'save_zeroes', metavar = 'save_zeros',
                      type = str, default = '',
                      help = 'R|Save the list of files with zero file size')
  parser.add_argument('-C', '--save-corrupted', dest = 'save_corrupted', metavar = 'save_corrupted',
                      type = str, default = '',
                      help = 'R|Save the list of corrupted files')
  parser.add_argument('-j', '--file-idx', dest = 'file_idx', metavar = 'number', type = int,
                      default = -1,
                      help = 'R|Check files at specified index (default: all files)')
  parser.add_argument('-v', '--verbose', dest = 'verbose', action = 'store_true', default = False,
                      help = 'R|Enable verbose printout')
  args = parser.parse_args()

  if args.verbose:
    logging.getLogger().setLevel(logging.DEBUG)

  if not os.path.isdir(args.output_directory):
    raise parser.error("Directory %s does not exist" % args.output_directory)

  if args.generate_jobs and not os.path.isdir(args.generate_jobs):
    raise parser.error("Directory %s does not exist" % args.generate_jobs)

  if (args.file_idx < 0 or not args.filter) and args.check_every_event:
    raise parser.error("Checking all files for data corruption is extremely slow! "
                       "Use -j/--file-idx and -f/--filter options on specific files if "
                       "you really need to check such files!")

  if args.save_corrupted and not args.check_every_event:
    logging.warning("The flag -C/--save-corrupted has no effect w/o -c/--check-every-event option")

  hdfs_system = hdfs()
  filetracker = FileTracker()

  paths_unchecked = obtain_paths(hdfs_system, args.path)
  excluded_paths  = obtain_paths(hdfs_system, args.exclude_path)

  # check if the given paths actually exist
  paths = [hdfs_system.get_path_info(path) for path in paths_unchecked]
  invalid_paths = filter(lambda path: not path.is_dir(), paths)
  if invalid_paths:
    raise parser.error('The following paths do not exist: %s' % ', '.join(invalid_paths))
  for path in paths:
    path.sparent_depth = path.depth

  # load the dictionaries
  meta_dict = load_dict(args.meta_dictionary, "meta_dictionary")
  sum_events = load_dict(args.meta_dictionary, "sum_events")
  process_names = { entry['process_name_specific'] : dbs_name for dbs_name, entry in meta_dict.items() }
  crab_strings  = { entry['crab_string']           : dbs_name for dbs_name, entry in meta_dict.items() }
  for key, entry in meta_dict.items():
    entry['located'] = False
  for key_arr in sum_events:
    for key in key_arr:
      if key not in process_names:
        raise ValueError("No such key in meta_dictionary: %s" % key)

  # set up the regex object
  name_regex = re.compile(args.filter)

  # process the directory structure of each path
  paths_to_traverse = []
  while paths:
    path = paths.pop(0)
    if path in excluded_paths:
      logging.info("Skipping path {path} since it is in the exclusion list".format(path = path.name))
      continue
    if args.depth > 0 and (path.depth - path.sparent_depth) >= args.depth:
      continue
    if path.basename in process_names:
      is_match = name_regex.match(meta_dict[process_names[path.basename]]['process_name_specific'])
      if is_match:
        if args.generate_jobs:
          paths_to_traverse.append(path.name_fuse)
        else:
          traverse_single(
            hdfs_system, meta_dict, path, process_names[path.basename], args.histogram,
            args.check_every_event, filetracker, args.file_idx
          )
    elif path.basename in crab_strings:
      is_match = name_regex.match(meta_dict[crab_strings[path.basename]]['process_name_specific'])
      if is_match:
        if args.generate_jobs:
          paths_to_traverse.append(path.name_fuse)
        else:
          traverse_double(
            hdfs_system, meta_dict, path, crab_strings[path.basename], args.histogram,
            args.check_every_event, filetracker, args.file_idx
          )
    else:
      entries = hdfs_system.get_dir_entries(path)
      entries_dirs = filter(
        lambda entry: entry.is_dir() and os.path.basename(entry.name_fuse) not in ["failed", "log"] and \
                      not any(map(
                        lambda path_to_traverse: entry.name_fuse.startswith(path_to_traverse),
                        paths_to_traverse
                      )),
        entries
      )
      for entry in entries_dirs:
        if entry not in paths:
          if entry.sparent_depth < 0:
            entry.sparent_depth = path.sparent_depth
          logging.debug(
            "Adding entry {entry} ({sparent_depth}/{depth})".format(
              entry         = entry.name,
              sparent_depth = entry.sparent_depth,
              depth         = entry.depth,
            )
          )
          paths.append(entry)

  if args.generate_jobs:
    commands = {}
    for arg in vars(args):
      args_attr = getattr(args, arg)
      if args_attr:
        option_key = ''
        option_default = None
        for option in parser._optionals._actions:
          if option.dest == arg:
            option_key = option.option_strings[0]
            option_default = option.default
            break
        if not option_key:
          raise ValueError("Internal error: inconsistencies in ArgumentParser!")
        if args_attr != option_default:
          if type(args_attr) is not bool:
            if type(args_attr) is list:
              commands[option_key] = ' '.join(map(str, args_attr))
            else:
              commands[option_key] = str(args_attr)
    for path_idx in range(len(paths_to_traverse)):
      commands_cp = copy.deepcopy(commands)
      commands_cp['-p'] = paths_to_traverse[path_idx]
      commands_cp['-N'] = 'dict.py.%i' % path_idx
      for key in ['-z', '-Z', '-C']:
        if key in commands_cp:
          commands_cp[key] = '%s.%i' % (commands_cp[key], path_idx)
      del commands_cp['-J']
      print(' '.join([sys.argv[0]] + [k + ' ' + v for k, v in commands_cp.items()]))
    sys.exit(0)

  # we need to post-process the meta dictionary
  for key, entry in meta_dict.items():
    if not name_regex.match(entry['process_name_specific']):
      continue
    if entry['located']:
      process_paths(meta_dict, key)
  for key_arr in sum_events:
    event_sum = 0
    missing_keys = []
    for meta_key, meta_entry in meta_dict.items():
      if meta_entry['process_name_specific'] in key_arr:
        if not meta_entry['located']:
          missing_keys.append(meta_entry['process_name_specific'])
        else:
          event_sum += meta_entry['nof_events']
    if 0 < len(missing_keys) < len(key_arr):
      raise ValueError("Could not find all samples to compute the number of events: %s" % \
                       ', '.join(missing_keys))
    for meta_key, meta_entry in meta_dict.items():
      if meta_entry['process_name_specific'] in key_arr:
        meta_entry['nof_events'] = event_sum

  output = jinja2.Template(header_str).render(
    command   = ' '.join(sys.argv),
    dict_name = args.output_dict_name,
  ) if not args.skip_header else ''
  for key, entry in meta_dict.items():
    if not name_regex.match(entry['process_name_specific']):
      continue
    if entry['located']:
      path_entries_arr = []
      for path_entry in meta_dict[key]['local_paths']:
        path_entries_arr.append(jinja2.Template(path_entry_str).render(
          path      = path_entry['path'],
          selection = path_entry['selection'],
          blacklist = path_entry['blacklist'], #TODO: format properly
        ))
      is_mc = meta_dict[key]['type'] == 'mc'
      output += jinja2.Template(dictionary_entry_str).render(
        dict_name                       = args.output_dict_name,
        dbs_name                        = key,
        sample_type                     = meta_dict[key]['type'],
        sample_category                 = meta_dict[key]['sample_category'],
        process_name_specific           = meta_dict[key]['process_name_specific'],
        nof_files                       = meta_dict[key]['nof_files'],
        nof_events                      = meta_dict[key]['nof_events'],
        use_HIP_mitigation_bTag         = meta_dict[key]['use_HIP_mitigation_bTag'],
        use_HIP_mitigation_mediumMuonId = meta_dict[key]['use_HIP_mitigation_mediumMuonId'],
        use_it                          = meta_dict[key]['use_it'],
        xsection                        = meta_dict[key]['xsection'] if is_mc else None,
        genWeight                       = meta_dict[key]['genWeight'],
        triggers                        = meta_dict[key]['triggers'],
        reHLT                           = meta_dict[key]['reHLT'],
        paths                           = '\n'.join(path_entries_arr),
      ) + '\n\n'
    else:
      logging.warning("Could not locate paths for {key}".format(key = key))

  dictionary_path = os.path.join(args.output_directory, args.generate_python)
  with open(dictionary_path, 'w') as f:
    f.write(output)
  logging.info("Wrote the dictionary to {path}".format(path = dictionary_path))

  if filetracker.zero_file_size:
    logging.info("The following files had file size of zero:\n{zero_fs}".format(
      zero_fs = '\n'.join(filetracker.zero_file_size),
    ))
    if args.save_zeroes:
      zeroes_path = os.path.join(args.output_directory, args.save_zeroes)
      with open(zeroes_path, 'w') as f:
        f.write('\n'.join(filetracker.zero_file_size) + '\n')
      logging.info("Saved the list of files with zero file size to {path}".format(
        path = zeroes_path,
      ))
  if filetracker.zombie_files:
    logging.info("The following files were zombies:\n{zombies}".format(
      zombies = '\n'.join(filetracker.zombie_files),
    ))
    if args.save_zombies:
      zombies_path = os.path.join(args.output_directory, args.save_zombies)
      with open(zombies_path, 'w') as f:
        f.write('\n'.join(filetracker.zombie_files) + '\n')
      logging.info("Saved the list of zombie files to {path}".format(path = zombies_path))
  if filetracker.corrupted_files:
    logging.info("The following files were corrupted:\n{corrupted}".format(
      corrupted = '\n'.join(filetracker.corrupted_files),
    ))
    if args.save_corrupted:
      corrupted_path = os.path.join(args.output_directory, args.save_corrupted)
      with open(corrupted_path, 'r') as f:
        f.write('\n'.join(filetracker.corrupted_files) + '\n')
      logging.info("Saved the list of corrupted files to {path}".format(path = corrupted_path))