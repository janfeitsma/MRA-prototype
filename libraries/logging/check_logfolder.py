#!/usr/bin/env python3

'''
System test case for MRA logger.
Intended to run at the end of the entire MRA testsuite.
(To support parallel test execution by bazel, this script will wait until other tests are finished.)

All logging produced during MRA test suite goes into log folder /tmp/testsuite_mra_logging.

This script looks inside, checks things like
* there should be enough log files (at least one per component)
* all log levels should be covered (error, info, debug etc)
* timestamps must be 'fresh'
'''

# python modules
import unittest
import os
import time
from datetime import datetime


TESTSUITE_LOG_FOLDER = '/tmp/testsuite_mra_logging'


class TestCheckLogFolder(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        # wait until folder is not growing/changing anymore, which indirectly signals all other testcases run by bazel/ctest are done
        assert(os.path.isdir(TESTSUITE_LOG_FOLDER))
        wait_for_folder_stable(TESTSUITE_LOG_FOLDER, 0.5, 30.0)
        cls._folder_contents = get_folder_contents(TESTSUITE_LOG_FOLDER, True)

    def test_filecount(self):
        # there should be enough log files (at least one per component)
        self.assertGreater(len(self._folder_contents.keys()), 6)

    def test_fresh(self):
        allowed_seconds = 30.0
        column_values = self._get_column(0, sort=True)
        t_format = '[%Y-%m-%dT%H:%M:%S.%f]'
        t_start = datetime.strptime(column_values[0], t_format)
        t_end = datetime.strptime(column_values[-1], t_format)
        now = datetime.now()
        self.assertGreater(t_end, t_start)
        self.assertLess((now - t_start).total_seconds(), allowed_seconds)
        self.assertLess((now - t_end).total_seconds(), allowed_seconds)

    def test_loglevels(self):
        # all log levels should be possible (error, info, debug etc)
        # NOTE: this test suite should not be sensitive to whatever logging the components may or may not produce,
        # instead see newer test in component test_mra_logger that control what is being logged
        column_values = self._get_column(2, uniq=True, strip='[]')
        self.assertTrue('info' in column_values)

    def test_line_truncation(self):
        # lines should be truncated if too long
        # example log line:
        # [2023-09-10T09:01:52.568021] [12/12/MRA:FalconsGetballIntercept] [info] [tick.cpp:24,tick] timestamp: 2023-09-10T09:01:52.548215475Z
        # the first 4 columns are not counted, can vary in size a bit
        # the remainder is formatted using vsnprintf, using 4096 buffer
        # so the total should be well below 4300
        expected_max_line_length = 4300
        max_line_length = 0
        longest_line = None
        for obj in self._folder_contents.values():
            for line in obj[1]:
                if len(line) > max_line_length:
                    longest_line = line
                    max_line_length = len(line)
        self.assertLess(len(longest_line), expected_max_line_length)
        # assuming there is a longest line that has been truncated
        self.assertTrue(longest_line.endswith('...\n'))

    def _get_column(self, idx, sort=False, uniq=False, strip=[]):
        result = []
        for obj in self._folder_contents.values():
            for line in obj[1]:
                w = line.split(' ')[idx]
                for s in strip:
                    w = w.replace(s, '')
                result.append(w)
        if sort:
            return sorted(result)
        if uniq:
            return sorted(list(set(result)))
        return result


def wait_for_folder_stable(folder_path, interval=0.5, timeout=30.0):
    """
    Wait until the specified folder stops changing.
    Args:
        folder_path (str): The path to the folder to monitor.
        interval (float, optional): The interval (in seconds) between checks.
    """
    t0 = time.time()
    previous_state = None
    while time.time() - t0 < timeout:
        time.sleep(interval)
        current_state = str(get_folder_contents(folder_path))
        if current_state == previous_state:
            return # converged OK
        previous_state = current_state
    raise Exception(f'folder contents did not converge in time ({timeout}s)')


def get_folder_contents(folder_path, load_all=False):
    """
    Get a dict representing the state of the folder.
    Keys: filenames (base).
    Values: tuples (numbytes, lines)
    Lines is only set when load_all is enabled.
    """
    result = {}
    for root, _, files in os.walk(folder_path):
        for file in files:
            if not file.endswith('.log'):
                continue
            file_path = os.path.join(root, file)
            if load_all:
                with open(file_path) as fh:
                    result[file] = (os.path.getsize(file_path), fh.readlines())
            else:
                result[file] = (os.path.getsize(file_path))
    return result


if __name__ == '__main__':
    # default enable verbosity
    unittest.main(argv=['ignored', '-v'])

