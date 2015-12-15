# python3

import os
import sys
import glob
import argparse
import shutil


__script_version__ = '1.0.0'


def error(msg):
    """ Display an error message and terminate. """
    msg = str(msg)
    sys.stderr.write("Error: %s\n" % msg)
    sys.exit(True)

def inform(msg):
    """ Display an information message. """
    msg = str(msg)
    sys.stdout.write("Inform: %s\n" % msg)

def newline():

    sys.stdout.write("\n")


# ------------------------------------------------------------------------------
# Enviroment
# ------------------------------------------------------------------------------
class Enviroment(object):
  """docstring for Enviroment"""
  def __init__(self, **kwargs):

    self.name = 'ns3_hx8352_lib'

    self.root_dir = os.path.abspath( kwargs.get('rootpath', '.') )
    self.objects_dir = os.path.abspath( kwargs.get('objectspath', '.\\.objs') )
    self.outpath = os.path.abspath( kwargs.get('outpath', '.') )
    self.verbose = kwargs.get( 'verbose', False )

  def dirs(self, **kwargs):
    """ get all filtered dirs from gived root """

    # get root dir
    root_dir = kwargs.get('root', self.root_dir)

    # create out list with root dir element
    out = [ os.path.abspath(root_dir) ]

    for _root, _dirs, _files in os.walk(root_dir):
      if '\.' not in _root:
        for d in _dirs:
          if '.' not in d:
            out.append(os.path.join(_root, d))

    return out

  def files(self, **kwargs):
    """ """

    root_dir = kwargs.get('root', self.root_dir)
    ext = kwargs.get('ext', 'False')

    out = []
    if ext:
      for _root, _dirs, _files in os.walk(root_dir):
        for f in _files:
          if f.endswith(ext):
            out.append( os.path.join(_root, f) )

    return [os.path.abspath(f) for f in out]

  def create_obj_dir(self):
    odir = os.path.abspath(self.objects_dir)
    os.makedirs(odir, mode=0x777)

  def clean(self):
    odir = os.path.abspath(self.objects_dir)
    if os.path.exists(odir):
      shutil.rmtree(odir)


# ------------------------------------------------------------------------------
# BuildBase
# ------------------------------------------------------------------------------
class BuildBase(object):
  """docstring for NS3_BuildBase"""
  def __init__(self, env):
    self.env = env

  def run(self):
    """ run """

    # prepare to compile
    self._prepare()

    # format cmd
    cmd = self._formatted_cmd()

    # inform user
    inform('Start proccess...')
    if self.env.verbose:
      inform(cmd)

    # --- run compile process, get final state
    state = os.system(cmd)

    # post compile
    self._final()


    if state != 0:
      error('Failed!')
    else:
      inform('Successfull.')


# ------------------------------------------------------------------------------
# Compiler
# ------------------------------------------------------------------------------
class Compiler(BuildBase):
  """docstring for Compiler"""
  def __init__(self, env, defs):
    super(Compiler, self).__init__(env)

    self._cmd = 'arm-none-eabi-gcc'

    self._defines = []
    self._defines += defs

    self._options = [
        '-mcpu=cortex-m3',
        '-mthumb',
        '-Wall',
        '-ffunction-sections',
        '-g',
        '-O3',
        '-c',
        # '-std=gnu99',
        # '--pedantic'
     ]

  def _prepare(self):
    """ prepare to run compiler """

    # env clean
    self.env.clean()

    # env create objects dir
    self.env.create_obj_dir()

    # change os root path to objects dir
    os.chdir(self.env.objects_dir)

  def _final(self):
    """ post compile event """
    # change os root path to user root dir
    os.chdir(self.env.root_dir)

  def _formatted_cmd(self, **kwargs):
    # cmd
    s = self._cmd + ' '
    # options
    s += ' '.join( self._options ) + ' '
    # defines
    s += ' '.join( [ '-D' + d for d in self._defines ] ) + ' '
    # includes
    s += ' '.join( [ '-I' + i for i in self.env.dirs() ] ) + ' '
    # files
    s += ' '.join( self.env.files( ext='.c' ) )
    return s


# ------------------------------------------------------------------------------
# Link
# ------------------------------------------------------------------------------
class Link(BuildBase):
  """ docstring for Link """
  def __init__(self, env):
    super(Link, self).__init__(env)

    self._cmd = 'arm-none-eabi-ar rvs'

    self._pre_options = [ env.name + '.a' ]
    self._post_options = []

  def _prepare(self):
    """ prepare to run compiler """
    # create dest dir
    if not os.path.exists(self.env.outpath):
      os.makedirs(self.env.outpath, mode=0x777)

    # change os to dest dir
    os.chdir(self.env.outpath)

  def _final(self):
    """ post compile event """
    # change os root path to user root dir
    os.chdir(self.env.root_dir)

  def _formatted_cmd(self):
    # cmd
    s = self._cmd + ' '
    # pre options
    s += ' '.join( self._pre_options ) + ' '
    # files
    s += ' '.join( self.env.files( root=self.env.objects_dir, ext='.o' ) ) + ' '
    # post options
    s += ' '.join( self._post_options )
    return s


# ------------------------------------------------------------------------------
# main
# ------------------------------------------------------------------------------
def main():

    # --- --- build

    # --- create enviroment
    envir = Enviroment( rootpath = os.path.dirname(__file__),
                        outpath = os.path.dirname(__file__),
                        verbose = True )

    # inform user
    inform( 'Start build - [ %s.a ]' % envir.name )
    newline()

    # --- create compiler, run.
    compiler = Compiler( envir, [] )
    compiler.run()

    # --- create link, run.
    link = Link( envir )
    link.run()


# ------------------------------------------------------------------------------
# program start here
# ------------------------------------------------------------------------------
if __name__ == '__main__':
    sys.exit(main())