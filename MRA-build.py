#!/usr/bin/env python3

'''
Build and optionally test. 

Default uses bazel build system.

Examples:
* run a full build
  $ ./MRA-build.py
* display the commands to run a full build and test
  $ ./MRA-build.py -n -t
     bazel build --color=yes //...
     bazel test //... --test_output all --nocache_test_results
* commands to build only one component
  $ ./MRA-build.py -n -s alive
     bazel build --color=yes //robotsports/proof_is_alive
'''

# python modules
import sys
import os
import glob
import pathlib
import subprocess
import argparse



MRA_ROOT = pathlib.Path(__file__).parent.resolve()
DEBUG_OPTIONS = '--subcommands --verbose_failures --sandbox_debug'
TEST_OPTIONS = '--test_output all --nocache_test_results'
DEFAULT_SCOPE = '...'
DEFAULT_NUM_PARALLEL_JOBS = 4 # TODO guess? building is nowadays quite memory-intensive ... easy to lock/swap


class BazelBuilder():
    def __init__(self, verbose: bool = True, debug: bool = False, dryrun: bool = False):
        self.verbose = verbose
        self.debug = debug
        self.dryrun = dryrun
    def run(self, clean: bool = False, test: bool = False, scope: str = DEFAULT_SCOPE, jobs: int = DEFAULT_NUM_PARALLEL_JOBS) -> None:
        """
        Perform the work:
        1. clean (optional)
        2. build
        3. test (optional)
        """
        if clean:
            self.run_clean()
        self.run_build(scope, jobs)
        if test:
            self.run_test(scope)
    def run_clean(self) -> None:
        self.run_cmd('bazel clean --color=yes')
    def run_build(self, scope: list, jobs: int = DEFAULT_NUM_PARALLEL_JOBS) -> None:
        cmd_parts = ['bazel', 'build', '--jobs', str(jobs)]
        if self.debug:
            cmd_parts.append(DEBUG_OPTIONS)
        for s in scope:
            self.run_cmd(' '.join(cmd_parts + ['--color=yes', f'//{s}']))
    def run_test(self, scope: list) -> None:
        for s in scope:
            cmd = f'bazel test //{s} ' + TEST_OPTIONS
            self.run_cmd(cmd)
    def run_cmd(self, cmd: str) -> None:
        extra_opts = {}
        if self.dryrun:
            print(cmd)
            return
        if self.verbose:
            print(f'running command: {cmd}')
        else:
            extra_opts = {'capture_output': True}
        r = subprocess.run(cmd, shell=True, **extra_opts)
        if r.returncode != 0:
            if not self.verbose:
                print('STDOUT:\n{}\n\nSTDERR:\n{}\n'.format(r.stdout.decode(), r.stderr.decode()))
            print(f'command "{cmd}" failed with returncode {r.returncode}')
            # no need to raise an Exception with a long uninteresting stacktrace
            sys.exit(1)


def is_component(d: str) -> bool:
    """Guess if a folder is a component. Allow nesting."""
    return os.path.isdir(os.path.join(d, 'interface'))


def find_components() -> list:
    """Find all component folders."""
    result = glob.glob(os.path.join('components/**'), recursive=True) # relative paths
    result = [d for d in result if is_component(d)]
    return result


def resolve_scope(iscope : str) -> list:
    """
    Resolve build- and optional test scope as a list of strings to pass to bazel, one at a time.
    Make a guess on user input, which may be a comma separated list of strings. See general examples.
    """
    if iscope == '...':
        return ['...'] # full scope, default
    components = find_components()
    result = set()
    for s in iscope.split(','):
        candidates = [c for c in components if c.endswith(s)]
        if len(candidates) == 1:
            result.add(candidates[0] + '/...')
        else:
            raise Exception(f'failed to resolve component for "{s}", candidates: ' + str(candidates))
    return list(result)


def main(**kwargs) -> None:
    """Perform the work."""
    os.chdir(MRA_ROOT)
    b = BazelBuilder(verbose = not kwargs.get('quiet'), debug = kwargs.get('debug'), dryrun = kwargs.get('dryrun'))
    scope = resolve_scope(kwargs.get('scope'))
    b.run(clean = kwargs.get('clean'), test = kwargs.get('test'), scope = scope, jobs = kwargs.get('jobs'))


def parse_args(args: list) -> argparse.Namespace:
    """Use argparse to parse command line arguments."""
    descriptionTxt = __doc__
    exampleTxt = ""
    class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
        pass
    parser = argparse.ArgumentParser(description=descriptionTxt, epilog=exampleTxt, formatter_class=CustomFormatter)
    parser.add_argument('-s', '--scope', help='build scope, comma-separated lists of (brief) component names', type=str, default=DEFAULT_SCOPE)
    parser.add_argument('-c', '--clean', help='start with cleaning', action='store_true')
    parser.add_argument('-t', '--test', help='also run tests', action='store_true')
    parser.add_argument('-n', '--dryrun', help='only print commands', action='store_true')
    parser.add_argument('-d', '--debug', help='enable some debug flags', action='store_true')
    parser.add_argument('-j', '--jobs', help='number of parallel build jobs (threads) to run', type=int, default=DEFAULT_NUM_PARALLEL_JOBS)
    parser.add_argument('-q', '--quiet', help='suppress output on what is happening', action='store_true')
    return parser.parse_args(args)


if __name__ == '__main__':
    main(**vars(parse_args(sys.argv[1:])))

