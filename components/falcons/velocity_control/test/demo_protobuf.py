#!/usr/bin/env python3

'''
Demo and test script to show python protobuf bindings.

Adding some test coverage around protobuf python build.

If it works for this component, then others are also likely covered.
'''

# python modules
import json

# component modules
import params_pb2



def main():
    # create an object of type Params
    p = params_pb2.Params()
    # load default values from json
    #TODO
    # print
    print(p)


if __name__ == '__main__':
    main()

