import sys, os
import inspect
from types import FunctionType, ModuleType


IMPORT_SUCCESS = False
try:
    import extendedlogging
    IMPORT_SUCCESS = True
except ImportError:
    p = '/home/jan/hobby/extendedlogging'
    if os.path.isdir(p):
        sys.path.append(p)
        import extendedlogging
        IMPORT_SUCCESS = True
    else:
        print('WARNING: could not find extendedlogging module')


# MAGIC!
# automatically apply the @extendedlogging.traced decorator to everything,
# without having to put it explicitly in the code

THIS_MODULE_NAME = __name__


def get_parent_module(look_for = 'import ' + THIS_MODULE_NAME):
    # Get the frame where the import was called
    found_frame = None
    for (it, s) in enumerate(inspect.stack()):
        if s.code_context is not None:
            for c in s.code_context:
                if look_for in c: # dirty assumption ...
                    found_frame = s.frame
                    break
    if found_frame is None:
        raise Exception('failed to detect parent module, was looking for ' + look_for)
    # Get the parent's module
    parent_module = inspect.getmodule(s.frame)
    return parent_module.__name__ if parent_module else '__main__'


def decorate_module(m):
    # Iterate over the members of the module
    for name, obj in inspect.getmembers(sys.modules[m]):
        if inspect.isfunction(obj) or inspect.isclass(obj):
            if hasattr(obj, '__module__') and obj.__module__ == m:
                setattr(sys.modules[m], name, extendedlogging.traced(obj))


def local_imports(entry_module):
    d = os.path.dirname(sys.modules[entry_module].__file__)
    result = []
    for mname, module in sys.modules.items():
        if hasattr(module, '__file__'):
            fname = module.__file__
            if isinstance(fname, str):
                md = os.path.dirname(module.__file__)
                if d == md and mname is not THIS_MODULE_NAME and mname[0] != '_':
                    result.append(mname)
    return result


def auto_decorate():
    entry_module = get_parent_module()
    decorate_module(entry_module)
    # also dive into nested non-system imports which have the same folder
    for m in local_imports(entry_module):
        decorate_module(m)


if IMPORT_SUCCESS:
    # Now call the decorate_all function here so that the decorator is automatically applied
    auto_decorate()
    decorate_module(get_parent_module())
    # Configure
    extendedlogging.configure(tracing=True, thread_names=True)


