#!/usr/bin/env python3

'''
Plot the diagnostics field in Local as serialized opencv object.
'''

# python modules
import sys, os
import argparse
import json
import numpy as np
import cv2
import struct
from matplotlib import pyplot as plt

# protobuf import (needs a little hack)
sys.path.append('/home/jan/github/MRA-prototype/bazel-bin')
try:
    import Local_pb2
except:
    os.system("protoc -I=../../.. -I=../../../datatypes -I=interface --python_out=. interface/Local.proto")
    import Local_pb2
from datatypes import CvMat_pb2



def parse_args(args: list) -> argparse.Namespace:
    """
    Use argparse to parse command line arguments.
    """
    descriptionTxt = __doc__
    exampleTxt = ""
    class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
        pass
    parser = argparse.ArgumentParser(description=descriptionTxt, epilog=exampleTxt, formatter_class=CustomFormatter)
    parser.add_argument('datafile', help='data file to load')
    return parser.parse_args(args)


def load_image_from_tickbin(filename):
    # data is serialized, see MRA libraries/logging/logging.hpp dumpToFile
    # each protobuf object is an int (#bytes) followed by serialized protobuf bytes
    # dumped data:
    #    input
    #    params
    #    state (before)
    #    output
    #    local
    #    state (after)
    data = None
    with open(filename, "rb") as f:
        for it in range(5):
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
    local = Local_pb2.Local()
    local.ParseFromString(data)
    image = local.fitResultFloor
    print(f'{image.height} x {image.width}')
    return image


def main(args: argparse.Namespace) -> None:
    """
    Make the plot.
    """
    image = load_image_from_tickbin(args.datafile)

    np_data = np.frombuffer(image.data, dtype=np.uint8).reshape(image.height, image.width, -1)

    plt.imshow(cv2.cvtColor(np_data, cv2.COLOR_BGR2RGB))
    plt.axis("off")
    plt.show()


if __name__ == '__main__':
    main(parse_args(sys.argv[1:]))

