# python modules
import os
import sys

# protobuf imports
# TODO: clean this up somehow, run protoc --python_out by default, ...
SCRIPT_FOLDER = os.path.dirname(os.path.realpath(__file__))
MRA_BASE_FOLDER = os.path.realpath(os.path.join(SCRIPT_FOLDER, '../../../..'))
sys.path.append(f'{MRA_BASE_FOLDER}/bazel-bin')
from datatypes import CvMat_pb2

# HACK
sys.path.append(f'{MRA_BASE_FOLDER}/bazel-bin/components/falcons/localization-vision/test')
import pybind11_example_module



def compile_protobuf():
    os.system(f'protoc -I={MRA_BASE_FOLDER} -I={MRA_BASE_FOLDER}/datatypes -I=../interface --python_out=. ../interface/Input.proto')
    os.system(f'protoc -I={MRA_BASE_FOLDER} -I={MRA_BASE_FOLDER}/datatypes -I=../interface --python_out=. ../interface/Params.proto')
    os.system(f'protoc -I={MRA_BASE_FOLDER} -I={MRA_BASE_FOLDER}/datatypes -I=../interface --python_out=. ../interface/State.proto')
    os.system(f'protoc -I={MRA_BASE_FOLDER} -I={MRA_BASE_FOLDER}/datatypes -I=../interface --python_out=. ../interface/Output.proto')
    os.system(f'protoc -I={MRA_BASE_FOLDER} -I={MRA_BASE_FOLDER}/datatypes -I=../interface --python_out=. ../interface/Local.proto')



compile_protobuf()


import Input_pb2
import Params_pb2
import State_pb2
import Output_pb2
import Local_pb2

