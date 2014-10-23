/* ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 */

#include <stdexcept>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <iterator>

#include <zlib.h>

#define BAM_BLK_MAX (64u * 1024u)
#define IO_BLK_SIZE (1024u * 1024u)

template<typename T>
static T LE2Host(void const *const src)
{
    union {
        uint8_t ch[sizeof(T)];
        T v;
    } u;
    memcpy(reinterpret_cast<void *>(&u), src, sizeof(T));
#if BYTE_ORDER == LITTLE_ENDIAN
    return u.v;
#else
    T y = 0;
    unsigned j = sizeof(T);
    
    for (unsigned i = 0; i < sizeof(T); ++i)
        y = (y << 8) | u.ch[--j];
    
    return y;
#endif
}

class BAMFilePosType {
    uint64_t value;
public:
    BAMFilePosType(uint64_t const x = 0) : value(x) {}
    bool hasValue() const { return value != 0; }
    uint64_t fpos() const {
        return value >> 16;
    }
    uint16_t bpos() const {
        return (uint16_t)value;
    }
    friend bool operator <(BAMFilePosType const lhs, BAMFilePosType const rhs) {
        return rhs.value < rhs.value;
    }
    friend bool operator ==(BAMFilePosType const lhs, BAMFilePosType const rhs) {
        return rhs.value == rhs.value;
    }
};

template <>
BAMFilePosType LE2Host<BAMFilePosType>(void const *const src)
{
    return BAMFilePosType(LE2Host<uint64_t>(src));
}


typedef std::vector<BAMFilePosType> BAMFilePosTypeList;

class BAMFile;
class RefIndex;

class HeaderRefInfo
{
    friend class BAMFile;
    
    RefIndex const *index;
    std::string name;
    unsigned length;
    
    HeaderRefInfo(std::string const &Name, int32_t const Length)
    : name(Name), length(Length), index(0)
    {}
    size_t LoadIndex(char const data[], char const *const endp);
    void DropIndex();
public:
    ~HeaderRefInfo() {
        DropIndex();
    }
    BAMFilePosTypeList slice(unsigned const beg, unsigned const end) const;
    std::string const &getName() const {
        return name;
    }
    unsigned getLength() const {
        return length;
    }
};

class BAMRecord {
    unsigned size;
    uint8_t
        m_refID[4],
        m_pos[4],
        m_bin_mq_nl[4],
        m_flag_nc[4],
        m_l_seq[4],
        m_next_refID[4],
        m_next_pos[4],
        m_tlen[4],
        m_readname[1];
    
    uint8_t const *_cigar() const { return &m_readname[l_read_name()]; }
    uint8_t const *_seq() const { return _cigar() + 4 * nc(); }

    static size_t abs_min_size() {
        BAMRecord const *const dummy = 0;

        return (size_t)(dummy->m_readname - dummy->m_refID);
    }
    size_t min_size() const {
        uint8_t const *const end = &m_readname[l_read_name() + 4 * nc() + ((l_seq() + 1) >> 1) + l_seq()];
        return (size_t)(end - m_refID);
    }
    void const *endp() const {
        return (void const *)(m_refID + size);
    }

    friend class BAMFile;
    BAMRecord() {}

public:
    ~BAMRecord() {}

    int32_t refID() const { return LE2Host<int32_t>(m_refID); }
    int32_t pos() const { return LE2Host<int32_t>(m_pos); }
    uint8_t mq() const { return m_bin_mq_nl[1]; }
    uint8_t l_read_name() const { return m_bin_mq_nl[0]; }
    uint16_t flag() const { return LE2Host<uint32_t>(m_flag_nc) >> 16; }
    uint16_t nc() const { return LE2Host<uint32_t>(m_flag_nc) & 0xFFFF; }
    int32_t l_seq() const { return LE2Host<int32_t>(m_l_seq); }
    int32_t next_refID() const { return LE2Host<int32_t>(m_next_refID); }
    int32_t next_pos() const { return LE2Host<int32_t>(m_next_pos); }
    int32_t tlen() const { return LE2Host<int32_t>(m_tlen); }
    char const *readname() const { return (char const *)m_readname; }
    uint32_t cigar(unsigned const i) const {
        return LE2Host<uint32_t>(_cigar() + i * 4);
    }
    char seq(unsigned const i) const {
        static char const tr[] = "=ACMGRSVTWYHKDBN";
        uint8_t const b4na2 = _seq()[i >> 1];
        uint8_t const lo = b4na2 & 15;
        uint8_t const hi = b4na2 >> 4;
        return tr[(i & 1) ? lo : hi];
    }
    uint8_t const *qual() const { return (uint8_t const *)(_seq() + ((l_seq() + 1) >> 1)); }
    void const *extra() const { return (void const *)(qual() + l_seq()); }
    
    unsigned refLen() const {
        unsigned const n = nc();
        unsigned rslt = 0;
        
        for (unsigned i = 0; i < n; ++i) {
            uint32_t const op = cigar(i);
            int const code = op & 0x0F;
            int const len = op >> 4;
            switch (code) {
                case 0: /* M */
                case 2: /* D */
                case 3: /* N */
                case 7: /* = */
                case 8: /* X */
                    rslt += len;
            }
        }
        return rslt;
    }
    bool isSelfMapped() const {
        return ((flag() & 0x0004) != 0 || refID() < 0 || pos() < 0 || nc() == 0) ? false : true;
    }
    bool isMateMapped() const {
        int const FLAG = flag();
        
        return ((FLAG & 0x0001) == 0 || (FLAG & 0x0008) != 0 || next_refID() < 0 || next_pos() < 0) ? false : true;
    }
    
    void cigarString(std::string &rslt, bool const clipped, char const OPCODE[]) const {
        unsigned const n = nc();
        int last_len = 0;
        char last_code = 0;
        unsigned last_size = 0;
        
        rslt.resize(0);
        rslt.reserve(11*n);
        
        for (unsigned i = 0; i < n; ++i) {
            char buf[12];
            char *cur = buf + sizeof(buf);
            uint32_t const op = cigar(i);
            char const code = OPCODE[op & 0x0F];
            int len = op >> 4;
            
            if (last_code == code) {
                len += last_len;
                rslt.resize(last_size);
            }
            last_size = (unsigned)rslt.size();
            last_len = len;
            last_code = code;
            
            *--cur = '\0';
            *--cur = code;
            for ( ; ; ) {
                *--cur = len % 10 + '0';
                if ((len /= 10) == 0)
                    break;
            }
            if (!clipped || last_size != 0 || !(last_code == 'S' || last_code == 'H'))
	            rslt.append(cur);
        }
        if (clipped && (last_code == 'S' || last_code == 'H'))
            rslt.resize(last_size);
    }
    class OptionalField {
        char tag[2];
        char val_type;
        union {
            char scalar[1];
            struct {
                char type;
                char count[4];
                char value[1];
            } array;
        } value;
        
        static int type_size(int const type) {
            switch (type) {
                case 'A':
                case 'C':
                case 'c':
                    return 1;
                case 'S':
                case 's':
                    return 2;
                case 'F':
                case 'I':
                case 'i':
                    return 4;
                default:
                    return -1;
            }
        }
        
        int size(void const *const max) const {
            if (val_type == 'B') {
                int const elem_size = type_size(value.array.type);
                if (elem_size < 0)
                    return -1;
                int const elem_count = LE2Host<int32_t>(value.array.count);
                char const *end = &value.array.value[elem_size * elem_count];
                
                if (end > max)
                    return -1;
                return (int)(end - tag);
            }
            else if (val_type == 'Z' || val_type == 'H') {
                for (char const *cur = value.scalar; cur != max; ++cur) {
                    if (*cur == '\0')
                        return (int)((cur + 1) - tag);
                }
                return -1;
            }
            else {
                int const ts = type_size(val_type);
                return ts < 0 ? -1 : (int)(&value.scalar[type_size(val_type)] - tag);
            }
        }
        void const *next(void const *const max) const {
            int const bytes = size(max);
            if (bytes <= 0)
                return 0;
            return (void const *)(((char const *)this) + bytes);
        }
        OptionalField() {}
    public:
        char const *getTag() const {
            return tag;
        }
        char getValueType() const {
            if (val_type != 'B')
                return val_type;
            else
                return value.array.type;
        }
        int getElementCount() const {
            if (val_type != 'B')
                return 1;
            else
                return LE2Host<int32_t>(value.array.count);
        }
        int getElementSize() const {
            if (val_type == 'B')
                return type_size(value.array.type);
            else if (val_type != 'Z' && val_type != 'H')
                return type_size(val_type);
            else
                return (int)strlen(value.scalar);
        }
        char const *getRawValue() const {
            if (val_type == 'B')
                return value.array.value;
            else
                return value.scalar;
        }
        
        typedef OptionalField const constOptionalField;
        class const_iterator : public std::iterator<std::forward_iterator_tag, constOptionalField>
        {
            friend class BAMRecord;
            void const *cur;
            void const *const endp;
            
            const_iterator(void const *const init, void const *const last) : cur(init), endp(last) {}
        public:
            const_iterator &operator ++() {
                if (cur < endp)
                    cur = ((OptionalField const *)cur)->next(endp);
                if (!cur)
                    cur = endp;
                return *this;
            }
            OptionalField const &operator *() {
                return *((OptionalField const *)cur);
            }
            OptionalField const *operator ->() {
                return (OptionalField const *)cur;
            }
            friend bool operator ==(const_iterator const &a, const_iterator const &b) {
                return a.endp == b.endp && a.cur == b.cur;
            }
            friend bool operator !=(const_iterator const &a, const_iterator const &b) {
                return !(a == b);
            }
        };
    };
    OptionalField::const_iterator begin() const {
        return OptionalField::const_iterator(extra(), endp());
    }
    OptionalField::const_iterator end() const {
        return OptionalField::const_iterator(endp(), endp());
    }
};

class BAMRecordSource
{
public:
    virtual bool isGoodRecord(BAMRecord const &rec) {
        return false;
    }
    virtual BAMRecord const *Read() {
        return 0;
    }
    virtual void DumpSAM(std::ostream &oss, BAMRecord const &rec) const {
        
    }
};

class BAMFile : public BAMRecordSource {
    std::ifstream file;
    std::vector<HeaderRefInfo> references;
    std::map<std::string, unsigned> referencesByName;
    std::string headerText;

    std::ifstream::pos_type first_bpos;
    std::ifstream::pos_type bpos;   /* file position of bambuffer */
    std::ifstream::pos_type cpos;   /* file position of iobuffer  */
    z_stream zs;
    
    unsigned first_bam_cur;
    unsigned bam_cur;               /* current offset in bambuffer */
    
    Bytef iobuffer[2*IO_BLK_SIZE];
    Bytef bambuffer[BAM_BLK_MAX];
    
    unsigned FillBuffer(int const n);
    void ReadZlib(void);
    size_t ReadN(size_t N, void *Dst);
    size_t SkipN(size_t N);
    template <typename T> bool Read(size_t count, T *dst);
    int32_t ReadI32();
    bool ReadI32(int32_t &rslt);
    void InflateInit(void);
    void CheckHeaderSignature(void);
    void ReadHeader(void);
    void LoadIndexData(size_t const fsize, char const data[]);
    void LoadIndex(std::string const &filepath);
    
public:
    BAMFile(std::string const &filepath);
    
    void Seek(std::ifstream::pos_type const &new_bpos, unsigned new_bam_cur);
    void Rewind() {
        Seek(first_bpos, first_bam_cur);
    }
    virtual bool isGoodRecord(BAMRecord const &rec);
    virtual BAMRecord const *Read();
    
    unsigned countOfReferences() const {
        return (unsigned)references.size();
    }
    
    int getReferenceIndexByName(std::string const &name) const {
        std::map<std::string, unsigned>::const_iterator i = referencesByName.find(name);
        if (i != referencesByName.end())
            return i->second;
        else
            return -1;
    }
    
    HeaderRefInfo const &getRefInfo(unsigned const i) const {
        return references[i];
    }
    
    BAMRecordSource *Slice(std::string const &rname, unsigned start, unsigned last);

    void DumpSAM(std::ostream &oss, BAMRecord const &rec) const;
};

class BAMFileSlice : public BAMRecordSource {
    friend class BAMFile;
    
    BAMFile *const parent;
    BAMFilePosTypeList const index;
    unsigned const refID;
    unsigned const start;
    unsigned const end;
    BAMFilePosTypeList::const_iterator cur;

    void Seek(void)
    {
        BAMFilePosType const pos = *cur++;
        std::ifstream::pos_type const fpos = pos.fpos();
        uint16_t const bpos = pos.bpos();
        
        parent->Seek(fpos, bpos);
    }
    BAMFileSlice(BAMFile &p, unsigned const r, unsigned const s, unsigned const e, BAMFilePosTypeList const &i)
    : parent(&p)
    , refID(r)
    , start(s)
    , end(e)
    , index(i)
    {
        cur = index.begin();
        Seek();
    }
public:
    virtual bool isGoodRecord(BAMRecord const &rec) {
        return parent->isGoodRecord(rec);
    }
    virtual BAMRecord const *Read() {
        for ( ; ; ) {
            BAMRecord const *const current = parent->Read();
            
            if (!current)
                return 0;
            
            if (!current->isSelfMapped()) {
                delete current;
                continue;
            }
            unsigned const REF = current->refID();
            unsigned const POS = current->pos();

            if (REF != refID || POS >= end) {
                delete current;
                return 0;
            }
            unsigned const LEN = current->refLen();
            
            if (POS + LEN <= start) {
                delete current;
                continue;
            }
            return current;
        }
    }
    void DumpSAM(std::ostream &oss, BAMRecord const &rec) const {
        parent->DumpSAM(oss, rec);
    }
};
