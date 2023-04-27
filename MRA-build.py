#!/usr/bin/env python3

'''
Build and optionally test. 

Default uses bazel build system.
TODO: option for cmake?
'''

# python modules
import sys
import os
import pathlib
import subprocess
import argparse



MRA_ROOT = pathlib.Path(__file__).parent.resolve()
DEBUG_OPTIONS = '--subcommands --verbose_failures --sandbox_debug'
TEST_OPTIONS = '--test_output all --nocache_test_results'


class BazelBuilder():
    def __init__(self, verbose: str = True, debug: str = False):
        self.verbose = verbose
        self.debug = debug
    def run(self, clean: str = False, test: str = False) -> None:
        """
        Perform the work:
        1. clean (optional)
        2. build
        3. test (optional)
        """
        if clean:
            self.run_clean()
        self.run_build()
        if test:
            self.run_test()
    def run_clean(self) -> None:
        self.run_cmd('bazel clean --color=yes')
    def run_build(self) -> None:
        cmd_parts = ['bazel', 'build']
        if self.debug:
            cmd_parts.append(DEBUG_OPTIONS)
        cmd_parts += ['--color=yes', '//...']
        self.run_cmd(' '.join(cmd_parts))
    def run_test(self) -> None:
        cmd = f'bazel test //... ' + TEST_OPTIONS
        self.run_cmd(cmd)
    def run_cmd(self, cmd: str) -> None:
        extra_opts = {}
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


def main(**kwargs) -> None:
    """Perform the work."""
    os.chdir(MRA_ROOT)
    b = BazelBuilder(verbose = not kwargs.get('quiet'), debug = kwargs.get('debug'))
    b.run(clean = kwargs.get('clean'), test = kwargs.get('test'))


def parse_args(args: list) -> argparse.Namespace:
    """Use argparse to parse command line arguments."""
    descriptionTxt = __doc__
    exampleTxt = ""
    class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
        pass
    parser = argparse.ArgumentParser(description=descriptionTxt, epilog=exampleTxt, formatter_class=CustomFormatter)
    parser.add_argument('-c', '--clean', help='start with cleaning', action='store_true')
    parser.add_argument('-t', '--test', help='also run tests', action='store_true')
    parser.add_argument('-d', '--debug', help='enable some debug flags', action='store_true')
    parser.add_argument('-q', '--quiet', help='suppress output on what is happening', action='store_true')
    return parser.parse_args(args)


if __name__ == '__main__':
    main(**vars(parse_args(sys.argv[1:])))

