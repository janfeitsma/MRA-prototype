# python modules
import struct

# our modules
import protobuf2python


class TickData():

    def __init__(self, filename = None):
        if filename:
            self.load(filename)

    def load(self, filename):
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
        with open(filename, 'rb') as f:
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.input = protobuf2python.Input_pb2.Input()
            self.input.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.params = protobuf2python.Params_pb2.Params()
            self.params.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.state_before = protobuf2python.State_pb2.State()
            self.state_before.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.output = protobuf2python.Output_pb2.Output()
            self.output.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.local = protobuf2python.Local_pb2.Local()
            self.local.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.state_after = protobuf2python.State_pb2.State()
            self.state_after.ParseFromString(data)

