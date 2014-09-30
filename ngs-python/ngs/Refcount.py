from ctypes import byref, c_void_p

from . import NGS
from String import NGS_RawString

def RefcountRelease(ref):
    """Releases NGS-object imported from ngs-sdk
    
    :param ref: reference to refcounted NGS-object to be released. It's expected to be of type c_void_p
    :returns: None
    :throws: ErrorMsg
    """
    with NGS_RawString() as ngs_str_err:
        res = NGS.lib_manager.PY_NGS_RefcountRelease(ref, byref(ngs_str_err.ref))

def RefcountRawStringRelease(ref):
    """Releases raw string imported from ngs-sdk
    
    :param ref: reference to raw char string. It's expected to be of type c_char_p
    :returns: None
    :throws: ErrorMsg
    """
    with NGS_RawString() as ngs_str_err:
        res = NGS.lib_manager.PY_NGS_RawStringRelease(ref, byref(ngs_str_err.ref))

# def RefcountEngineRelease(ref):
    # """Releases NGS-object imported from ngs engine
    
    # :param ref: reference to refcounted NGS-object to be released. It's expected to be of type c_void_p
    # :returns: None
    # :throws: ErrorMsg
    # """
    # with NGSEngine_String() as ngs_str_err:
        # res = NGS.lib_manager.PY_NGS_Engine_RefcountRelease(ref, byref(ngs_str_err.ref))
        # check_res(res, ngs_str_err)


class Refcount:
    """ Base class for all refcounted objects imported from ngs-sdk
    """
    def __init__(self):
        self.init_members_with_null()
    
    def __del__(self):
        self.close()
            
    def __enter__(self):
        return self
    
    def __exit__(self, t, value, traceback):
        self.close()

    def close(self):
        if self.ref:
            RefcountRelease(self.ref)
            self.init_members_with_null()
            
    def init_members_with_null(self):
        self.ref = c_void_p()
