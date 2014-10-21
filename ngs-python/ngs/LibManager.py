from ctypes import cdll, c_char, c_int, c_char_p, c_int32, c_int64, c_double, POINTER, c_size_t, c_void_p, c_uint64, c_uint32
import os, tempfile, platform

from ErrorMsg import check_res_embedded

class LibManager:
    # lib_path_engine = "/home/ukrainch/centos/gcc/x86_64/rel/lib/libncbi-vdb.so"
    # lib_path_sdk = "/home/ukrainch/ncbi-outdir/ngs-sdk/x86_64/lib/libngs-sdk.so"
    
    c_lib_engine = None
    c_lib_sdk = None
    
    def _bind(self, c_lib, c_func_name_str, param_types_list, errorcheck):
        setattr(self, c_func_name_str, getattr(c_lib, c_func_name_str))
        func = getattr(self, c_func_name_str)
        func.argtypes = param_types_list
        func.restype = c_int
        if errorcheck:
            func.errcheck = errorcheck
        
    def bind_sdk(self, c_func_name_str, param_types_list):
        return self._bind(self.c_lib_sdk, c_func_name_str, param_types_list, check_res_embedded)
    
    def resolve_libpath(self, lib_filename):
        # Try to load so from the root of ngs package
        #full_path = os.path.join(os.path.dirname(__file__), lib_filename)
        full_path = lib_filename
        return full_path
        
        # TODO: cannot check path to dll with isfile()
        # waiting on VDB-1296 to modify this function or
        # get rid of it at all
        if not os.path.isfile(full_path):
            full_path = "./" + lib_filename # Try to load just by filename (CWD, PATH?)
            
        if not os.path.isfile(full_path):
            full_path = os.path.join(tempfile.gettempdir(), lib_filename) # Try to load from TMP
        
        # TODO: add more options here (download from ncbi, known path)
        
        if not os.path.isfile(full_path): # nothing worked - cannot resolve path
            raise RuntimeError("FAILED to resolve path for " + lib_filename)
        
        return full_path

    @staticmethod
    def get_lib_extension():
        if platform.system() == "Windows":
            return "dll"
        #elif platform.name() == "": # TODO: add mac os
        else:
            return "so"
    
    def initialize_ngs_bindings(self):
        if self.c_lib_engine and self.c_lib_sdk: # already initialized
            return

        lib_ext = "." + LibManager.get_lib_extension()
        
        libname_engine = "libncbi-vdb" + lib_ext
        libname_sdk = "libngs-sdk" + lib_ext

        path_engine = self.resolve_libpath(libname_engine)
        path_sdk = self.resolve_libpath(libname_sdk)
        
        self.c_lib_engine = cdll.LoadLibrary(path_engine)
        self.c_lib_sdk = cdll.LoadLibrary(path_sdk)

        ##############  ngs-engine imports below  ####################
        
        self._bind(self.c_lib_engine, "PY_NGS_Engine_ReadCollectionMake", [c_char_p, POINTER(c_void_p), POINTER(c_char), c_size_t], None)
        # self._bind(self.c_lib_engine, "PY_NGS_Engine_RefcountRelease", [c_void_p, POINTER(c_void_p)], None)
        # self._bind(self.c_lib_engine, "PY_NGS_Engine_StringData", [c_void_p, POINTER(c_char_p)], None)
        # self._bind(self.c_lib_engine, "PY_NGS_Engine_StringSize", [c_void_p, POINTER(c_size_t)], None)
        
        ##############  ngs-sdk imports below  ####################

        # Common
        
        self._bind(self.c_lib_sdk, "PY_NGS_StringGetData",    [c_void_p, POINTER(c_char_p)], None)
        self._bind(self.c_lib_sdk, "PY_NGS_StringGetSize",    [c_void_p, POINTER(c_size_t)], None)
        self._bind(self.c_lib_sdk, "PY_NGS_RawStringRelease", [c_void_p, POINTER(c_void_p)], None)
        self._bind(self.c_lib_sdk, "PY_NGS_RefcountRelease",  [c_void_p, POINTER(c_void_p)], None)
        
        # ReadCollection

        self.bind_sdk("PY_NGS_ReadCollectionGetName",           [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetReadGroups",     [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetReadGroup",      [c_void_p, c_char_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetReferences",     [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetReference",      [c_void_p, c_char_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetAlignment",      [c_void_p, c_char_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetAlignments",     [c_void_p, c_uint32, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetAlignmentCount", [c_void_p, c_uint32, POINTER(c_uint64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetAlignmentRange", [c_void_p, c_uint64, c_uint64, c_uint32, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetRead",           [c_void_p, c_char_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetReads",          [c_void_p, c_uint32, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetReadCount",      [c_void_p, c_uint32, POINTER(c_uint64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadCollectionGetReadRange",      [c_void_p, c_uint64, c_uint64, c_uint32, POINTER(c_void_p), POINTER(c_void_p)])

        # Alignment
        
        self.bind_sdk("PY_NGS_AlignmentGetAlignmentId",               [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetReferenceSpec",             [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetMappingQuality",            [c_void_p, POINTER(c_int32), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetReferenceBases",            [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetReadGroup",                 [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetReadId",                    [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetClippedFragmentBases",      [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetClippedFragmentQualities",  [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetAlignedFragmentBases",      [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetAlignmentCategory",         [c_void_p, POINTER(c_uint32), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetAlignmentPosition",         [c_void_p, POINTER(c_int64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetAlignmentLength",           [c_void_p, POINTER(c_uint64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetIsReversedOrientation",     [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetSoftClip",                  [c_void_p, c_uint32, POINTER(c_int32), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetTemplateLength",            [c_void_p, POINTER(c_uint64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetShortCigar",                [c_void_p, c_int, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetLongCigar",                 [c_void_p, c_int, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentHasMate",                      [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetMateAlignmentId",           [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetMateAlignment",             [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetMateReferenceSpec",         [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_AlignmentGetMateIsReversedOrientation", [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        
        self.bind_sdk("PY_NGS_AlignmentIteratorNext",                 [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        
        # Fragment
        
        self.bind_sdk("PY_NGS_FragmentGetFragmentId",        [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_FragmentGetFragmentBases",     [c_void_p, c_uint64, c_uint64, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_FragmentGetFragmentQualities", [c_void_p, c_uint64, c_uint64, POINTER(c_void_p), POINTER(c_void_p)])

        self.bind_sdk("PY_NGS_FragmentIteratorNext",         [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        
        # PileupEvent
        
        self.bind_sdk("PY_NGS_PileupEventGetReferenceSpec",          [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetReferencePosition",      [c_void_p, POINTER(c_int64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetMappingQuality",         [c_void_p, POINTER(c_int32), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetAlignmentId",            [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetAlignment",              [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetAlignmentPosition",      [c_void_p, POINTER(c_int64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetFirstAlignmentPosition", [c_void_p, POINTER(c_int64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetLastAlignmentPosition",  [c_void_p, POINTER(c_int64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetEventType",              [c_void_p, POINTER(c_uint32), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetAlignmentBase",          [c_void_p, POINTER(c_char), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetAlignmentQuality",       [c_void_p, POINTER(c_char), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetInsertionBases",         [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetInsertionQualities",     [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupEventGetDeletionCount",          [c_void_p, POINTER(c_uint32), POINTER(c_void_p)])

        self.bind_sdk("PY_NGS_PileupEventIteratorNext",              [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        
        # Pileup

        self.bind_sdk("PY_NGS_PileupGetReferenceSpec",     [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupGetReferencePosition", [c_void_p, POINTER(c_int64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupGetPileupEvents",      [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_PileupGetPileupDepth",       [c_void_p, POINTER(c_uint32), POINTER(c_void_p)])

        self.bind_sdk("PY_NGS_PileupIteratorNext",         [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        
        # ReadGroup
        
        self.bind_sdk("PY_NGS_ReadGroupGetName",       [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadGroupGetStatistics", [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        
        self.bind_sdk("PY_NGS_ReadGroupIteratorNext",  [c_void_p, POINTER(c_int), POINTER(c_void_p)])

        # Read
        
        self.bind_sdk("PY_NGS_ReadGetReadId",        [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadGetNumFragments",  [c_void_p, POINTER(c_uint32), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadGetReadCategory",  [c_void_p, POINTER(c_uint32), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadGetReadGroup",     [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadGetReadName",      [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadGetReadBases",     [c_void_p, c_uint64, c_uint64, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReadGetReadQualities", [c_void_p, c_uint64, c_uint64, POINTER(c_void_p), POINTER(c_void_p)])

        self.bind_sdk("PY_NGS_ReadIteratorNext",     [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        
        # Reference
        
        self.bind_sdk("PY_NGS_ReferenceGetCommonName",     [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetCanonicalName",  [c_void_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetIsCircular",     [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetLength",         [c_void_p, POINTER(c_uint64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetReferenceBases", [c_void_p, c_uint64, c_uint64, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetReferenceChunk", [c_void_p, c_uint64, c_uint64, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetAlignment",      [c_void_p, c_char_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetAlignments",     [c_void_p, c_uint32, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetAlignmentSlice", [c_void_p, c_int64, c_uint64, c_uint32, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetPileups",        [c_void_p, c_uint32, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_ReferenceGetPileupSlice",    [c_void_p, c_int64, c_uint64, c_uint32, POINTER(c_void_p), POINTER(c_void_p)])

        self.bind_sdk("PY_NGS_ReferenceIteratorNext",      [c_void_p, POINTER(c_int), POINTER(c_void_p)])
        
        # Statistics
        
        self.bind_sdk("PY_NGS_StatisticsGetValueType", [c_void_p, c_char_p, POINTER(c_uint32), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_StatisticsGetAsString",  [c_void_p, c_char_p, POINTER(c_void_p), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_StatisticsGetAsI64",     [c_void_p, c_char_p, POINTER(c_int64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_StatisticsGetAsU64",     [c_void_p, c_char_p, POINTER(c_uint64), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_StatisticsGetAsDouble",  [c_void_p, c_char_p, POINTER(c_double), POINTER(c_void_p)])
        self.bind_sdk("PY_NGS_StatisticsGetNextPath",  [c_void_p, c_char_p, POINTER(c_void_p), POINTER(c_void_p)])
