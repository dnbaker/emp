#ifndef BNS_RHTRAITS_H__
#define BNS_RHTRAITS_H__
#include "alphabet.h"

namespace bns {

enum InputType {
    DNA,
    PROTEIN,      // Treats all characters as valid
    PROTEIN20,    // Corresponds to AMINO20, which masks unexpected characters
    PROTEIN_3BIT, // Corresponds to SEB8, which can hold 22 in 64-bits and 42 in 128-bits
    PROTEIN_14,   // Corresponds to SEB14, which can hold up to 16 in 64 bits and 33 in 128-bits
    PROTEIN_6,    // Corresponds to SEB6, which can hold up to 24 in 64 bits and 49 in 128-bits
    DNA2,         // AT vs GC, corresponds to DNA2PYR
    DNAC,         // corresponds to DNA2METHYL, C vs otherwise
    PROTEIN_6_FRAME,
    PROTEIN8 = PROTEIN_3BIT,
    PROTEIN6 = PROTEIN_6,
    PROTEIN14 = PROTEIN_14
};

template<InputType rht> struct RHTraits {
    static constexpr size_t alphsize = 0;
    static constexpr size_t nper32 = 0;
    static constexpr size_t nper64 = 0;
    static constexpr size_t nper128 = 0;
    static constexpr const alph::Alphabet &table = alph::BYTES;
};
template<> struct RHTraits<DNA> {
    static constexpr size_t alphsize = 4;
    static constexpr size_t nper32 = 16;
    static constexpr size_t nper64 = 32;
    static constexpr size_t nper128 = 64;
    static constexpr const alph::Alphabet &table = alph::DNA4;
};
template<> struct RHTraits<PROTEIN> {
    static constexpr size_t alphsize = 256;
    static constexpr size_t nper32 = 4;
    static constexpr size_t nper64 = 8;
    static constexpr size_t nper128 = 16;
    static constexpr const alph::Alphabet &table = alph::BYTES;
};

static constexpr inline size_t rh2n(InputType rht, size_t itemsize);
static inline std::string to_string(InputType it);


template<typename KmerT>
KmerT rhmask(InputType it, int k) {
    if(it == DNA) return 1 + (static_cast<KmerT>(-1) >> (sizeof(KmerT) * 8 - (k << 1)));
    if(it == DNA2 || it == DNAC) return 1 + (static_cast<KmerT>(-1) >> (sizeof(KmerT) * 8 - k));
    if(it == PROTEIN20) return std::pow(20, k);
    if(it == PROTEIN_6) return std::pow(6, k);
    if(it == PROTEIN_3BIT) return std::pow(8, k);
    if(it == PROTEIN_14) return std::pow(14, k);
    if(it == PROTEIN) return std::pow(256, k);
    return KmerT(-1);
}
static constexpr inline size_t mul(InputType it) {
    switch(it) {
        case DNA: return 4;
        case PROTEIN: return 256;
        case PROTEIN20: return 20;
        case PROTEIN_3BIT: return 8;
        case PROTEIN_14: return 14;
        case PROTEIN_6: return 6;
        case DNAC: case DNA2: return 2;
        case PROTEIN_6_FRAME: default: return 4;
    }
    return 2; //Should never happen
}

template<> struct RHTraits<PROTEIN20> {
    static constexpr size_t alphsize = 20;
    static constexpr size_t nper32 = 7;
    static constexpr size_t nper64 = 14;
    static constexpr size_t nper128 = 29;
    static constexpr const alph::Alphabet &table = alph::AMINO20;
};
template<> struct RHTraits<PROTEIN_3BIT> {
    static constexpr size_t alphsize = 8;
    static constexpr size_t nper32 = 10;
    static constexpr size_t nper64 = 22;
    static constexpr size_t nper128 = 42;
    static constexpr const alph::Alphabet &table = alph::SEB8;
};
template<> struct RHTraits<PROTEIN_14> {
    static constexpr size_t alphsize = 14;
    static constexpr size_t nper32 = 8;
    static constexpr size_t nper64 = 16;
    static constexpr size_t nper128 = 33;
    static constexpr const alph::Alphabet &table = alph::SEB14;
};
template<> struct RHTraits<PROTEIN_6> {
    static constexpr size_t alphsize = 6;
    static constexpr size_t nper32 = 12;
    static constexpr size_t nper64 = 24;
    static constexpr size_t nper128 = 49;
    static constexpr const alph::Alphabet &table = alph::SEB6;
};
template<> struct RHTraits<DNA2> {
    static constexpr size_t alphsize = 2;
    static constexpr size_t nper32 = 32;
    static constexpr size_t nper64 = 32;
    static constexpr size_t nper128 = 64;
    static constexpr const alph::Alphabet &table = alph::DNA2KETAMINE;
};
template<> struct RHTraits<DNAC> {
    static constexpr size_t alphsize = 2;
    static constexpr size_t nper32 = 32;
    static constexpr size_t nper64 = 32;
    static constexpr size_t nper128 = 64;
    static constexpr const alph::Alphabet &table = alph::DNA2METHYL;
};

static constexpr const int8_t *rh2lp(InputType rht) {
    switch(rht) {
#define CASE(x) case x: return RHTraits<x>::table.data();
        CASE(DNA) CASE(DNA2) CASE(PROTEIN) CASE(PROTEIN20) CASE(PROTEIN_3BIT) CASE(PROTEIN_14) CASE(PROTEIN_6) CASE(DNAC)
        default: ;
#undef CASE
    }
    return RHTraits<PROTEIN>::table.data();
}

static constexpr inline size_t rh2n(InputType rht, size_t itemsize) {
    switch(rht) {
#define CASE(x) case x: return itemsize == 16 ? RHTraits<x>::nper128: itemsize == 8 ? RHTraits<x>::nper64: RHTraits<x>::nper32;
        CASE(DNA) CASE(DNA2) CASE(PROTEIN) CASE(PROTEIN20) CASE(PROTEIN_3BIT) CASE(PROTEIN_14) CASE(PROTEIN_6) CASE(DNAC)
        case PROTEIN_6_FRAME: return -1;
#undef CASE
    }
    return 0;
}

static inline std::string to_string(InputType rht) {
    switch(rht) {
        case DNA: return "DNA";
        case PROTEIN: return "PROTEIN";
        case PROTEIN20: return "PROTEIN20";
        case PROTEIN_3BIT: return "PROTEIN_3BIT";
        case PROTEIN_14: return "PROTEIN_14";
        case PROTEIN_6: return "PROTEIN_6";
        case PROTEIN_6_FRAME: return "PROTEIN_6_FRAME";
        case DNA2: return "DNA2_AT_GC";
        case DNAC: return "DNA_C_ATG";
        default:;
    }
    return "unknown";
}
}

#endif /* BNS_RHTRAITS_H__ */
