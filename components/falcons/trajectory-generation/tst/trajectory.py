#!/usr/bin/env python3

'''
Python command line interface around the TrajectoryGeneration model (C++).

Default output mode is a brief report.
Other modes: pretty-print as table, produce json, make plots.

Examples (with #comments):

    # generate a very simple x-only trajectory, print a brief report
    $ ./trajectory.py --tx 0.1 brief
    input:
        target setpoint position x  :    0.1000
    parameters overrule:
        n/a
    result:
        duration                    :    0.4250
        number of samples           :   17
        final position x            :    0.0747
        final velocity vx           :    0.2231
    # note how the final sample stops abruptly before reaching target
    # this is because default VelocityControl tolerance is 3cm,
    # so the iteration stops after 7cm of requested 10cm

    # overrule tolerance parameter to become much tighter
    $ ./trajectory.py --tx=0.1 -p '{"deadzone": {"toleranceXY": 0.001}}'
    input:
        target setpoint position x  :    0.1000
    parameters overrule:
        {"deadzone": {"toleranceXY": 0.001}}
    result:
        duration                    :    0.6500
        number of samples           :   26
        final position x            :    0.0991
        final velocity vx           :    0.0086

    # rotate, cross the angle wrap-around at pi, show values in a rz-only table
    $ ./trajectory.py --rz=3.13 --trz=-3.13 --dofs rz table 
         iter         t    pos.rz    vel.rz
    =======================================
            1    0.0250    3.1309    0.0375
            2    0.0500    3.1328    0.0750
            3    0.0750    3.1356    0.1125
            4    0.1000    3.1394    0.1500
            5    0.1250   -3.1391    0.1875
            6    0.1500   -3.1335    0.2250
'''

# python modules
import sys
import argparse
import subprocess
import json
import re
import random



#BINARY = 'bazel-bin/components/falcons/trajectory-generation/tst/trajectory-generator'
BINARY = 'bazel run --ui_event_filters=-info,-stdout,-stderr --noshow_progress //components/falcons/trajectory-generation/tst:trajectory-generator'
DOFS = ('x', 'y', 'rz', 'vx', 'vy', 'vrz')
INPUT_TYPES = (('t', 'target setpoint'), ('', 'robot initial'), )
POSVEL = ['position', 'velocity']



def parse_args(args: list) -> argparse.Namespace:
    """
    Use argparse to parse command line arguments.
    """
    descriptionTxt = __doc__
    exampleTxt = ""
    class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
        pass
    parser = argparse.ArgumentParser(description=descriptionTxt, epilog=exampleTxt, formatter_class=CustomFormatter)
    for (prefix, descr) in INPUT_TYPES:
        for dof in DOFS:
            posvel = POSVEL['v' in dof]
            parser.add_argument(f'--{prefix}{dof}', help=f'{descr} {posvel} {dof}', type=float, default=0.0)
    parser.add_argument('--maxticks', help='iteration limit', type=int, default=1000)
    parser.add_argument('-r', help='apply a random delta with given scaling size to all parameters', metavar='randomscale', type=float, default=0.0)
    parser.add_argument('--mp', help='motion profile to use', metavar='motionprofile', type=int, default=0)
    parser.add_argument('-p', help='json overrule string for VelocityControl parameters', metavar='vc-parameters', type=str, default='{}')
    parser.add_argument('--dofs', help='which degrees of freedom to show, comma-separated', type=str, default='x,y,rz')
    parser.add_argument('--exe', help=argparse.SUPPRESS, default=BINARY) # intended for testsuite only
    parser.add_argument('mode', help='output mode', choices=['brief', 'table', 'plot', 'json'], default='brief', nargs='?')
    return parser.parse_args(args)


def main(args: argparse.Namespace) -> None:
    """
    Generate a trajectory.
    """
    # apply random deltas
    for (k,v) in vars(args).items():
        if isinstance(v, float) and k not in ['r']:
            setattr(args, k, v + random.gauss(0, args.r))

    # construct command with arguments
    inputjson = '{"setpoint": {"position": {"x": ' + str(args.tx) + ', "y": ' + str(args.ty) + ', "rz": ' + str(args.trz) + '},'
    inputjson += '"velocity": {"x": ' + str(args.tvx) + ', "y": ' + str(args.tvy) + ', "rz": ' + str(args.tvrz) + '}},'
    inputjson += '"worldstate": {"robot": {"position": {"x": ' + str(args.x) + ', "y": ' + str(args.y) + ', "rz": ' + str(args.rz) + '},'
    inputjson += '"velocity": {"x": ' + str(args.vx) + ', "y": ' + str(args.vy) + ', "rz": ' + str(args.vrz) + '}}}}'
    paramjson = '{"maxTicks": ' + str(args.maxticks) + ', "vcParamsJsonStr": ' + json.dumps(args.p) + '}'
    cmd = f"{args.exe} '{inputjson}' '{paramjson}'"
    #print(cmd)

    # call the binary
    r = subprocess.run(cmd, shell=True, capture_output=True)
    if r.returncode != 0:
        print('command: ' + cmd)
        print(r.stderr.decode())
        print(f'ERROR: trajectory-generator exited with code {r.returncode}')
        sys.exit(r.returncode)

    # convert output to json object
    output = r.stdout.decode()
    json_data = json.loads(output)
    dofs = args.dofs.split(',')

    # handle output mode
    if args.mode == 'json':
        print(json_data)
    elif args.mode == 'brief':
        report_brief(args, json_data)
    else: # table or plot
        add_missing_zeros(json_data)
        if args.mode == 'table':
            print(make_ascii_table(json_data, dofs))
        elif args.mode == 'plot':
            make_plots(json_data, dofs)


def report_brief(args: argparse.Namespace, json_data) -> None:
    print('input:')
    have_input = False
    for (prefix, descr) in INPUT_TYPES:
        for dof in DOFS:
            posvel = POSVEL['v' in dof]
            key = f'{prefix}{dof}'
            value = getattr(args, key)
            if value != 0.0:
                have_input = True
                ss = f'{descr} {posvel} {dof:3s}'
                print(f'    {ss:28s}: {value:9.4f}')
    if not have_input:
        print('    n/a')
    print('parameters overrule:')
    if args.p == '{}':
        print('    n/a')
    else:
        print('    ' + args.p)
    print('result:')
    if not have_input:
        print('    n/a')
    else:
        print('    duration                    : {:9.4f}'.format(json_data['output']['duration']))
        print('    number of samples           : {:4d}'.format(json_data['output']['numTicks']))
        for dof in ('x', 'y', 'rz', 'vx', 'vy', 'vrz'):
            posvel = ['position', 'velocity']['v' in dof]
            try:
                value = json_data['output']['final'][posvel][dof.lstrip('v')]
                print(f'    final {posvel} {dof:3s}          : {value:9.4f}')
            except:
                pass


def add_missing_zeros(json_data) -> None:
    # this makes printing the table and producing plots easier
    # (protobuf v3 tends to omit zeros)
    for cat in POSVEL:
        for dof in ('x', 'y', 'rz'):
            try:
                value = json_data['input']['setpoint'][cat][dof]
            except KeyError:
                json_data['input']['setpoint'][cat][dof] = 0.0
    for sample in json_data['output']['samples']:
        for cat in POSVEL:
            for dof in ('x', 'y', 'rz'):
                try:
                    value = sample[cat][dof]
                except KeyError:
                    sample[cat][dof] = 0.0


def make_ascii_table(json_data, dofs: list) -> str:
    result = ''
    columns = ['iter', 't']
    for cat in ['pos', 'vel']:
        for dof in dofs:
            columns.append(cat + '.' + dof)
    header_row = ' '.join(f'{c:>9s}' for c in columns)
    header_sep = '=' * len(header_row)
    result += header_row + '\n'
    result += header_sep + '\n'
    for (irow, sample) in enumerate(json_data['output']['samples']):
        data_row = '{:9d} {:9.4f}'.format(1+irow, sample['t'])
        for cat in POSVEL:
            for dof in dofs:
                value = sample[cat][dof]
                data_row += ' {:9.4f}'.format(value)
        result += data_row + '\n'
    if len(json_data['output']['samples']) > 20:
        result += header_sep + '\n'
        result += header_row + '\n'
    return result


def make_plots(json_data, dofs):
    import plots
    plots.make_plots(json_data, dofs)



if __name__ == '__main__':
    main(parse_args(sys.argv[1:]))

