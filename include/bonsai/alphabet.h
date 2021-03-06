#ifndef BNS_ALPHBET_H__
#define BNS_ALPHBET_H__
#include <array>
#include <map>
#include <cstdint>
#include <string>
#include <climits>
#include <type_traits>

namespace bns {
namespace alph {
using std::size_t;


template<typename VT=int8_t, size_t NCHAR=size_t(1) << (sizeof(VT) * CHAR_BIT)>
struct TAlphabet {
    static_assert(NCHAR > 1, "Nchar must be positive");
    static_assert(std::is_integral<VT>::value, "VT must be integral");
    const char *name;
    const char *setstr;
private:
    size_t nc;
public:
    bool padding;
    size_t nchars() const {return nc + 1;} // One for padding
    using LUType = std::array<VT, NCHAR>;
    LUType lut;
    static constexpr LUType make_lut(const char *s, const size_t nc, bool padding=false) {
        LUType arr{-1};
        int id = padding;
        size_t ci = 0;
        for(size_t i = 0; i < nc; ++i, ++id, ++ci) {
            while(s[ci] && s[ci] != ',') {
                const auto v = s[ci++];
                arr[v | 32] = arr[v & static_cast<uint8_t>(0xdf)] = id; // lower-case and upper-case
            }
        }
        while(s[ci]) {
            const auto v = s[ci++];
            arr[v | 32] = arr[v & static_cast<uint8_t>(0xdf)] = id; // lower-case and upper-case
        }
        // Handle Pyrrolysine (P) by mapping it to Lysine (K) if unhandled
        if(arr['P'] == VT(-1)) arr['P'] = arr['p'] = arr['K'];
        // Handle SelenoCysteine (U) by mapping it to Cysteine if unhandled
        if(arr['U'] == VT(-1)) arr['U'] = arr['u'] = arr['C'];
        return arr;
    }
    using SignedVT = typename std::make_signed<VT>::type;
    constexpr VT translate(VT x) const {return lut[static_cast<SignedVT>(x)];}
    constexpr VT *data() {return lut.data();}
    constexpr const VT *data() const {return lut.data();}
    static constexpr size_t size() {return NCHAR;}
    static constexpr size_t ncommas(const char *s) {
        size_t ret = 0, i = 0;
        while(s[i]) ret += (s[i] == ','), ++i;
        return ret;
    }
    constexpr TAlphabet(const TAlphabet &) = default;
    constexpr TAlphabet(TAlphabet &&) = default;
    constexpr TAlphabet(const char *name, const char *s, bool padding=false): name(name), setstr(s), nc(ncommas(s)), padding(padding), lut(make_lut(s, nc, padding)) {
    }
    static constexpr LUType emptylut(bool padding) {
        LUType tlut{-1};
        for(size_t i = 0; i < NCHAR; ++i) tlut[i] = i + padding;
        return tlut;
    }
    constexpr TAlphabet(bool padding=false): name("Bytes"), setstr(""), nc(NCHAR - 1 + padding), padding(padding), lut(emptylut(padding)) {
    }
};
struct Alphabet: public TAlphabet<int8_t> {
    template<typename...Args> constexpr Alphabet(Args &&...args): TAlphabet(std::forward<Args>(args)...) {}
};

// Protein Alphabets
static constexpr const Alphabet BYTES;
static constexpr const Alphabet AMINO20("Standard20", "A,C,D,E,F,G,H,I,K,L,M,N,P,Q,R,S,T,V,W,Y");

static constexpr const Alphabet SEB14("SE-B(14)", "A,C,D,EQ,FY,G,H,IV,KR,LM,N,P,ST,W");

static constexpr const Alphabet SEB10("SE-B(10)", "AST,C,DN,EQ,FY,G,HW,ILMV,KR,P");
static constexpr const Alphabet SEV10("SE-V(10)", "AST,C,DEN,FY,G,H,ILMV,KQR,P,W");
static constexpr const Alphabet SOLISD("Solis-D", "AM,C,DNS,EKQR,F,GP,HT,IV,LY,W");
static constexpr const Alphabet SOLISG("Solis-G", "AEFIKLMQRVW,C,D,G,H,N,P,S,T,Y");
static constexpr const Alphabet MURPHY("Murphy", "A,C,DENQ,FWY,G,H,ILMV,KR,P,ST");
static constexpr const Alphabet LIA10("Li-A(10)", "AC,DE,FWY,G,HN,IV,KQR,LM,P,ST");
static constexpr const Alphabet LIB10("Li-B(10)", "AST,C,DEQ,FWY,G,HN,IV,KR,LM,P");

static constexpr const Alphabet SEB8("SE-B(8)","AST,C,DHN,EKQR,FWY,G,ILMV,P");
static constexpr const Alphabet SEB6("SE-B(6)","AST,CP,DHNEKQR,FWY,G,ILMV");

static constexpr const Alphabet DAYHOFF("Dayhoff","AGPST,C,DENQ,FWY,HKR,ILMV");


// DNA alphabets

static constexpr const Alphabet DNA4("DNA4", "A,C,G,T");
static constexpr const Alphabet DNA5("DNA5", "A,C,G,T,NMRWSYKVHDB");

static constexpr const Alphabet DNA2KETAMINE("DNA2", "ACM,KGT"); // Amino/Ketones
static constexpr const Alphabet DNA2PYRPUR("DNA2", "AGR,YCT"); // Purines/Pyrimidines
static constexpr const Alphabet DNA2METHYL("DNAMETH", "C,AGT"); // Purines/Pyrimidines


// Source: Reference
// Edgar, RC (2004) Local homology recognition and distance measures in linear time using compressed amino acid alphabets, NAR 32(1), 380-385. doi: 10.1093/nar/gkh180
static const std::map<std::string, const Alphabet *> CAMAP {
    {"AMINO20", &AMINO20},
    {"AMINO", &AMINO20},
    {"PROTEIN", &AMINO20},
    {"SEB8", &SEB8},
    {"SEB10", &SEB10},
    {"SEB14", &SEB14},
    {"SEV10", &SEV10},
    {"MURPHY", &MURPHY},
    {"LIA10", &LIA10},
    {"LIB10", &LIB10},
    {"SEB6", &SEB6},
    {"DAYHOFF", &DAYHOFF},

    {"DNAMETH", &DNA2METHYL},
    {"C", &DNA2METHYL},
    {"KETO", &DNA2KETAMINE},
    {"PURPYR", &DNA2PYRPUR},

    {"DNA4", &DNA4},
    {"DNA", &DNA4},

    {"DNA5", &DNA5}
};

} // alph

using alph::Alphabet;
using alph::BYTES;
using alph::AMINO20;
using alph::SEB14;
using alph::SEB10;
using alph::SEB6;
using alph::SEB8;
using alph::SEV10;
using alph::SOLISD;
using alph::SOLISG;
using alph::MURPHY;
using alph::LIA10;
using alph::LIB10;
using alph::DAYHOFF;
using alph::DNA5;
using alph::DNA4;
using alph::DNA2KETAMINE;
using alph::DNA2PYRPUR;


} // namespace bns


#endif /* BNS_ALPHBET_H__ */
