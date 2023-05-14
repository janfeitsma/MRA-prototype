#!/usr/bin/env python3

'''
Python command line interface around the TrajectoryGeneration model.
'''

# python modules
import sys
import argparse
import subprocess


#BINARY = 'bazel-bin/components/falcons/trajectory-generation/tst/trajectory-generator'
BINARY = 'bazel run --ui_event_filters=-info,-stdout,-stderr --noshow_progress //components/falcons/trajectory-generation/tst:trajectory-generator'



def main(args: argparse.Namespace) -> None:
    """
    Generate a trajectory.
    """
    # TODO: check if everything is zero, then error?
    inputjson = '{"setpoint": {"position": {"x": ' + str(args.tx) + ', "y": ' + str(args.ty) + ', "rz": ' + str(args.trz) + '}}}'
    paramjson = ''
    cmd = f"{BINARY} '{inputjson}' '{paramjson}'"
    #print(cmd)
    r = subprocess.run(cmd, shell=True, capture_output=True)
    if r.returncode != 0:
        print(r.stderr)
        print('ERROR: trajectory-generator exited with code {r.returncode}')
        sys.exit(r.returncode)
    print(r.stdout.decode())
    # TODO: plot option


def parse_args(args: list) -> argparse.Namespace:
    """
    Use argparse to parse command line arguments.
    """
    descriptionTxt = __doc__
    exampleTxt = ""
    class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
        pass
        #def __init__(self, prog):
        #    # slightly widen first column to avoid ugly line breaks
        #    width = 30
        #    argparse.ArgumentDefaultsHelpFormatter.__init__(self, prog, max_help_position=width)
        #    argparse.RawDescriptionHelpFormatter.__init__(self, prog, max_help_position=width)
    parser = argparse.ArgumentParser(description=descriptionTxt, epilog=exampleTxt, formatter_class=CustomFormatter)
    parser.add_argument('--tx', help='target setpoint x', type=float, default=0.0)
    parser.add_argument('--ty', help='target setpoint y', type=float, default=0.0)
    parser.add_argument('--trz', help='target setpoint rz', type=float, default=0.0)
    return parser.parse_args(args)


if __name__ == '__main__':
    main(parse_args(sys.argv[1:]))

