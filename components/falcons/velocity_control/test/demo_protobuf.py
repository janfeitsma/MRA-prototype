#!/usr/bin/env python3

'''
Demo and test script to show python protobuf bindings.

Adding some test coverage around protobuf python build.

If it works for this component, then others are also likely covered.
'''


# python modules
import json
from google.protobuf import json_format

# component modules
import components.falcons.velocity_control.interface.Params_pb2 as Params_pb2



def main():
    file_path = 'components/falcons/velocity_control/interface/DefaultParams.json'
    with open(file_path, 'r') as file:
        json_data = json.load(file)
    protobuf_message = Params_pb2.Params()
    json_format.ParseDict(json_data, protobuf_message)
    print(f'Successfully read Params from {file_path}, first lines follow:')
    print('\n'.join(str(protobuf_message).splitlines()[:7]))


if __name__ == '__main__':
    main()

