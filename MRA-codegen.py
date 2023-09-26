#!/usr/bin/env python3

'''
Generate files for all components, so that 'bazel build' can do its job.


This script is intended to help MRA developers to create a new component.
Developers first need to define the name (folder) and interface (.proto files).

Example output for a new component:
    file components/falcons/getball_fetch/interface/BUILD has been copied (and modified) from base/codegen/template_interface.BUILD
    file components/falcons/getball_fetch/BUILD has been copied (and modified) from base/codegen/template_implementation.BUILD
    file components/falcons/getball_fetch/CMakeLists.txt has been copied (and modified) from base/codegen/template_CMakeLists.txt
    file components/falcons/getball_fetch/FalconsGetballFetch.hpp has been copied (and modified) from base/codegen/template_instance.hpp
    file components/falcons/getball_fetch/tick.cpp has been copied (and modified) from base/codegen/template_tick.cpp
    file components/falcons/getball_fetch/test.cpp has been copied (and modified) from base/codegen/template_test.cpp


Example output for a finished component:
    file components/falcons/getball_fetch/interface/BUILD already exists, skipping (content unchanged)
    file components/falcons/getball_fetch/BUILD already exists, skipping (content unchanged)
    file components/falcons/getball_fetch/CMakeLists.txt already exists, skipping (content unchanged)
    file components/falcons/getball_fetch/FalconsGetballFetch.hpp already exists, skipping (content unchanged)
    file components/falcons/getball_fetch/tick.cpp already exists, skipping (overwrite disabled)
    file components/falcons/getball_fetch/test.cpp already exists, skipping (overwrite disabled)

Notes for new components:
    For a new component interface/Input.proto and interface/Output.proto are required.
    The generator performs a check if these files have the correct package name (MRA.<Component-parent><Component>
    A CMakeLists.txt will be created in the component directory.
    In the component-parent folder the CMakeLists.txt must be updated to build the new component
    (an example is <MRA-root>/components/robotsports/CMakeLists.txt)

'''

# python modules
import os
import sys
import shutil
import re
import filecmp
import glob
import pathlib
import argparse



MRA_ROOT = pathlib.Path(__file__).parent.resolve()
MRA_COMPONENTS_ROOT = 'components'
DEFAULT_VERBOSE_LEVEL = 1


def component_name_underscore(component: str) -> str:
    """Mangle (relative) component name. Example: falcons/getball_intercept -> FALCONS_GETBALL_INTERCEPT"""
    return component.replace('/', '_').replace('-', '_').upper()

def component_name_dash(component: str) -> str:
    """Mangle (relative) component name. Example: falcons/getball_intercept -> falcons-getball-intercept"""
    return component.replace('/', '-').replace('_', '-').lower()

def component_name_camelcase(component: str) -> str:
    """Mangle (relative) component name. Example: falcons/getball_intercept -> FalconsGetballIntercept"""
    return ''.join([s.capitalize() for s in re.split('/|-|_', component)])

def grep(pat: str, fname: str) -> list:
    """Mimic grep utility. Return matching lines."""
    result = []
    for line in open(fname, 'r'):
        if re.search(pat, line):
            result.append(line)
    return result


class ComponentGenerator():
    """
    Perform all code generation operations for given component.
    Given component must be a string relative to component root (incl. folder nesting).
    """
    def __init__(self, component: str, verbosity: int = DEFAULT_VERBOSE_LEVEL):
        self.component = component # example: falcons/getball_intercept
        self.verbosity = verbosity
        self.cname_underscore = component_name_underscore(component) # example: FALCONS_GETBALL_INTERCEPT
        self.cname_camelcase = component_name_camelcase(component) # example: FalconsGetballIntercept
        self.cname_dash = component_name_dash(component) # example: falcons-getball-intercept
        self.template_folder = MRA_ROOT / 'base/codegen'
        self.components_folder = MRA_ROOT / 'components'
        self.component_folder = self.components_folder / self.component
        self.interface_folder = os.path.join(self.component_folder, 'interface')
        self.analyze()

    def check_component_name_valid(self):
        """Ensure component name is valid, for instance no dash character ... (causes problem with protoc --python_out)"""
        if '-' in self.component:
            raise Exception(f'illegal component name, no dash (-) allowed in {self.component}')

    def check_component_interface_valid(self):
        """Ensure component interface (.proto files) are valid."""
        # TODO: warn in case other than all_interface_parts .proto files are detected?
        for required_interface_part in ['Input', 'Output']:
            if not required_interface_part in self.interface_parts:
                raise Exception(f'incomplete component interface: missing file {required_interface_part}.proto')

    def analyze(self) -> None:
        """Analyze component."""
        # interface parts, max 5
        self.all_interface_parts = ['Input', 'Params', 'State', 'Output', 'Local']
        self.interface_parts = [p for p in self.all_interface_parts if os.path.isfile(os.path.join(self.component_folder, 'interface', p + '.proto'))]
        self.check_component_name_valid()
        self.check_component_interface_valid()
        # Params.proto also requires DefaultParams.json (workaround for protobuf v3 not supporting default values anymore ...)
        if 'Params' in self.interface_parts:
            f = os.path.join(self.component_folder, 'interface', 'DefaultParams.json')
            if not os.path.isfile(f):
                raise Exception(f'required file is missing: {f}')
            # TODO: strict checks on correctness & completeness? (allow empty file?)
        # dependencies to other libraries / components w.r.t. MRA_ROOT
        self.component_dependencies = []
        self.library_dependencies = []
        self.internal_dependencies = []
        deps_file = os.path.join(self.component_folder, 'dependencies')
        if os.path.isfile(deps_file):
            for dep in [ll.strip() for ll in open(deps_file).readlines()]:
                if len(dep) == 0:
                    continue
                if os.path.isdir(os.path.join(self.component_folder, dep.split(':')[0])):
                    self.internal_dependencies.append(dep)
                    continue
                if not os.path.isdir(dep):
                    raise Exception(f'invalid dependency, folder not found: "{dep}" in file "{deps_file}"')
                if dep.startswith('components'):
                    self.component_dependencies.append(dep)
                elif dep.startswith('libraries'):
                    self.library_dependencies.append(dep)
                else:
                    raise Exception(f'invalid dependency, expected dependency to start with either "components" or "libraries": "{dep}" in file "{deps_file}"')
        # check if package naming is consistent
        for p in self.interface_parts:
            pf = os.path.join(self.component_folder, 'interface', p + '.proto')
            expected_line = 'package MRA.{};'.format(self.cname_camelcase)
            if not grep(expected_line, pf):
                raise Exception(f'missing or incorrect package name: expected to find "{expected_line}" in file "{pf}"')

    def run(self) -> None:
        """Perform all the work for current component."""
        self.changed = False
        self.make_replace_dict()
        self.handle_interface_bazel_build()
        self.handle_implementation_bazel_build()
        self.handle_header_datatypes_hpp()
        self.handle_header_hpp()
        self.generate_copy_files_unless_existing()
        self.handle_cmakelists_txt()
        return self.changed

    def notify_copy(self, src: str, tgt: str, level: int = 1) -> None:
        """Print an info message about a copy action."""
        self.changed = True
        # hide full path, only report paths w.r.t. MRA root
        src = str(src).replace(str(MRA_ROOT) + '/', '')
        tgt = str(tgt).replace(str(MRA_ROOT) + '/', '')
        msg = 'file {} has been copied'.format(tgt)
        if not filecmp.cmp(src, tgt):
            msg += ' (and modified)'
        msg += ' from ' + str(src)
        if level <= self.verbosity:
            print(msg)

    def notify(self, msg: str, level: int = 1) -> None:
        """Print an info message, or not."""
        msg = str(msg).replace(str(MRA_ROOT) + '/', '')
        if level <= self.verbosity:
            print(msg)

    def check_copy_and_modify(self, src: str, tgt: str, replace = None, overwrite: bool = True, check_diff: bool = True) -> None:
        """
        Helper function:
        * check if target file already exists
        * load content from source file as template
        * apply replace dict
        * write to target file
        """
        #print(f'debug copy_and_modify({src}, {tgt})')
        content = open(src).read()
        if os.path.isdir(tgt):
            tgt = os.path.join(tgt, os.path.basename(src))
        if os.path.isfile(tgt) and not overwrite:
            self.notify('file {} already exists, skipping (overwrite disabled)'.format(tgt), level=2)
            return
        tgt_dir = pathlib.Path(tgt).parent
        tgt_dir.mkdir(exist_ok=True, parents=True)
        replace_dict = {}
        if not replace is False:
            replace_dict = self.replace_dict.copy()
            if isinstance(replace, dict):
                replace_dict.update(replace)
            # specifics:
            replace_dict['SOURCEFILE'] = os.path.basename(src)
        # iterate until converged
        change = True
        max_it = 20
        it = 0
        while change:
            it += 1
            old_content = content
            for (before, after) in replace_dict.items():
                content = content.replace(before, after)
            change = content != old_content
            if it > max_it:
                #fh.write(content) # write anyway, for inspection
                #print(replace_dict)
                raise Exception('something went wrong in iterative search-and-replace, trying to write ' + tgt)
        # check content against existing
        if os.path.isfile(tgt) and check_diff:
            existing_content = open(tgt).read()
            if content == existing_content:
                self.notify('file {} already exists, skipping (content unchanged)'.format(tgt), level=2)
                return
        with open(tgt, 'w') as fh:
            fh.write(content)
        self.notify_copy(src, tgt)

    def make_protobuf_includes(self) -> str:
        """Snippet of code to include protobuf-generated header files into component.hpp."""
        result = ""
        c = self.cname_camelcase
        cc = self.component
        for p in self.all_interface_parts:
            if p in self.interface_parts:
                result += f"#include \"MRA_COMPONENTS_ROOT/{cc}/interface/{p}.pb.h\"\n"
        return result

    def make_tick_includes(self) -> str:
        """Snippet of code to include header files into tick.cpp."""
        result = []
        for cc in self.component_dependencies:
            ccc = component_name_camelcase(cc.lstrip('components/'))
            result.append(f'#include "{ccc}.hpp"')
        # TODO: also handle self.library_dependencies?
        if len(result):
            return '\n// dependent (generated) component headers:\n' + '\n'.join(result) + '\n'
        return ''

    def make_protobuf_typedefs(self) -> str:
        """Snippet of code to typedef, improve readibility."""
        result = ""
        c = self.cname_camelcase
        for p in self.all_interface_parts:
            if p in self.interface_parts:
                result += f"typedef MRA::{c}::{p} {p}Type;\n"
            else:
                result += f"typedef google::protobuf::Empty {p}Type; // no .proto -> unused\n"
        return result

    def make_build_deps_interface(self) -> str:
        """Make a dict to be used when replacing content of template files during code generation."""
        result = []
        for dep in self.component_dependencies:
            result.append(f'"//{dep}/interface:interface_proto",')
        return '\n        '.join(result)

    def make_build_deps_implementation(self) -> str:
        """Make a dict to be used when replacing content of template files during code generation."""
        result = []
        for dep in self.component_dependencies:
            result.append(f'"//{dep}:implementation",')
        for dep in self.internal_dependencies:
            result.append(f'"//components/{self.component}/{dep}",')
        for dep in self.library_dependencies:
            result.append(f'"//{dep}",')
        return '\n        '.join(result)

    def make_cmake_deps_interface(self) -> str:
        """Make a dict to be used when replacing content of template files during code generation."""
        result = []
        for dep in self.interface_parts:
            result.append(f'    interface/{dep}.proto')
        return '\n'.join(result)

    def make_replace_dict(self) -> None:
        """Make a dict to be used when replacing content of template files during code generation."""
        self.replace_dict = {
            'MRA_COMPONENTS_ROOT': str(MRA_COMPONENTS_ROOT),
            'COMPONENT_REL_PATH': self.component,
            'COMPONENT_CPP_NAME_CAMELCASE': self.cname_camelcase,
            'COMPONENT_CPP_NAME_UNDERSCORE': self.cname_underscore,
            'DEPENDENT_HEADERS': self.make_tick_includes(),
            'PROTOBUF_HPP_TYPE_INCLUDES': self.make_protobuf_includes(),
            'PROTOBUF_HPP_TYPE_TYPEDEFS': self.make_protobuf_typedefs(),
            'CODEGEN_NOTE': 'this file was produced by MRA-codegen.py from SOURCEFILE',
            'BAZEL_INTERFACE_DEPENDENCIES': self.make_build_deps_interface(),
            'BAZEL_IMPLEMENTATION_DEPENDENCIES': self.make_build_deps_implementation(),
            'CMAKE_COMPONENT_TEST_NAME': self.cname_underscore.lower()+'_test',
            'CMAKE_COMPONENT_LIBRARY_NAME': self.cname_dash,
            'CMAKE_INTERFACE_DEPENDENCIES': self.make_cmake_deps_interface(),
        }

    def handle_interface_bazel_build(self) -> None:
        """Generate bazel BUILD for interface (protobuf)."""
        src = self.template_folder / 'template_interface.BUILD'
        tgt = os.path.join(self.interface_folder, 'BUILD') # don't use 'interface' or is_component gets confused
        self.check_copy_and_modify(src, tgt)

    def handle_implementation_bazel_build(self) -> None:
        """Generate bazel BUILD for the implementation."""
        src = self.template_folder / 'template_implementation.BUILD'
        tgt = os.path.join(self.component_folder, 'BUILD')
        self.check_copy_and_modify(src, tgt)

    def handle_cmakelists_txt(self) -> None:
        """Generate CMakeLists.txt for the ."""
        src = self.template_folder / 'template_CMakeLists.txt'
        tgt = os.path.join(self.component_folder, 'CMakeLists.txt')
        self.check_copy_and_modify(src, tgt, overwrite=False)

    def handle_header_hpp(self) -> None:
        """Generate C++ header file, using generated protobuf types and deriving from base class."""
        src = self.template_folder / 'template_instance.hpp'
        tgt = os.path.join(self.component_folder, self.cname_camelcase + '.hpp')
        self.check_copy_and_modify(src, tgt)

    def handle_header_datatypes_hpp(self) -> None:
        """Generate C++ header file which only exposes the generated protobuf datatypes."""
        src = self.template_folder / 'template_datatypes.hpp'
        tgt = os.path.join(self.component_folder, 'datatypes.hpp')
        self.check_copy_and_modify(src, tgt)

    def generate_copy_files_unless_existing(self) -> None:
        """Copy implementation file(s) to the component folder, create from template if not existing."""
        tgt = self.component_folder
        for f in ['tick.cpp', 'test.cpp']:
            src = self.template_folder / ('template_' + f)
            self.check_copy_and_modify(src, os.path.join(tgt, f), overwrite=False)


def is_component(d: str) -> bool:
    """Guess if a folder is a component. Allow nesting."""
    return os.path.isdir(os.path.join(d, 'interface'))


def find_components() -> list:
    """Find all component folders."""
    result = glob.glob(os.path.join(MRA_COMPONENTS_ROOT + '/**'), recursive=True) # relative paths
    result = [d for d in result if is_component(d)]
    result = [d.lstrip(MRA_COMPONENTS_ROOT + '/') for d in result] # strip components prefix
    return result


def main(verbosity: int = DEFAULT_VERBOSE_LEVEL, error_on_change: bool = False) -> None:
    """Perform the work for all detected components."""
    os.chdir(MRA_ROOT)
    components = find_components()
    num_components_changed = 0
    for cc in components:
        num_components_changed += ComponentGenerator(cc, verbosity).run()
    if error_on_change and num_components_changed:
        print(f'ERROR: code generator changed {num_components_changed} component' + ['', 's'][num_components_changed>1])
        sys.exit(1)


def parse_args(args: list) -> argparse.Namespace:
    """Use argparse to parse command line arguments."""
    descriptionTxt = __doc__
    exampleTxt = ""
    class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
        pass
    parser = argparse.ArgumentParser(description=descriptionTxt, epilog=exampleTxt, formatter_class=CustomFormatter)
    parser.add_argument('-e', '--error-on-change', help='error if any change is done (used in github action)', action='store_true')
    parser.add_argument('-v', '--verbosity', help='set verbosity level, 0 is entirely quiet, 2 is fully verbose', type=int, default=DEFAULT_VERBOSE_LEVEL)
    return parser.parse_args(args)


if __name__ == '__main__':
    main(**vars(parse_args(sys.argv[1:])))

