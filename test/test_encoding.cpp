#include "test/catch.hpp"
#include "lib/spacer.h"
#include "lib/encoder.h"
#include <algorithm>
#include <numeric>

// This file tests qmap, encoding, and decoding.

using namespace emp;

TEST_CASE( "Spacer encodes and decodes contiguous, unminimized seeds correctly.", "[contiguous]") {
    spvec_t v;
    const char *test_str("ACATGCTAGCATGCTGACTGACTGATCGATCGTA");
    SECTION("Add spaces") {
        std::string test(test_str);
        v = {1, 2}; // Skip 1 base after the first nucleotide, then skip two, then don't skip any.
        test.resize(34); // Just long enough to encode a k-31 with 3 total spaces in it.
        while(v.size() < 30) v.push_back(0);
        Spacer sp(31, 31, v);
        Encoder<lex_score> enc(&test[0], 34, sp, nullptr);
        REQUIRE(sp.s_.size() == 30);
        REQUIRE(std::accumulate(sp.s_.begin(), sp.s_.end(), 0u) == sp.s_.size() + std::accumulate(v.begin(), v.end(), 0));
        std::string to_use = test;
        to_use[1] = '-'; to_use[3] = '-'; to_use[4] = '-';
        REQUIRE(to_use != test);
        REQUIRE(sp.to_string(enc.next_kmer()) == to_use);
    }
    SECTION("next_minimizer rather than kmer") {
        std::string test(test_str);
        v = {1, 2};
        test.resize(34);
        while(v.size() < 30) v.push_back(0);
        Spacer sp(31, 31, v);
        Encoder<lex_score> enc(&test[0], 34, sp, nullptr);
        REQUIRE(sp.s_.size() == 30);
        REQUIRE(std::accumulate(sp.s_.begin(), sp.s_.end(), 0u) == sp.s_.size() + std::accumulate(v.begin(), v.end(), 0));
        std::string to_use = test;
        to_use[1] = '-'; to_use[3] = '-'; to_use[4] = '-';
        REQUIRE(to_use != test);
        REQUIRE(sp.to_string(enc.next_minimizer()) == to_use);
    }
    SECTION("small_genome") {
        v = {1, 2, 18};
        while(v.size() < 30) v.push_back(0);
        Spacer sp(31, 31, v);
        Encoder<lex_score> enc(nullptr, 34, sp, nullptr);
        gzFile fp(gzopen("test/small_genome.fa", "rb"));
        kseq_t *ks(kseq_init(fp));
        kseq_read(ks);
        enc.assign(ks);
        REQUIRE(!enc.has_next_kmer());
        kseq_read(ks);
        enc.assign(ks);
        REQUIRE(enc.has_next_kmer());
        kseq_destroy(ks);
        gzclose(fp);
    }
    SECTION("qmap") {
        for(auto window_size: {32, 55, 100, 300, 500}) {
            std::vector<uint64_t> minimizers;
            gzFile fp(gzopen("test/phix.fa", "rb"));
            kseq_t *ks(kseq_init(fp));
            kseq_read(ks);
            v = {1, 2, 18};
            size_t vsum(0); for(auto i: v) vsum += i;
            while(v.size() < 30) v.push_back(0);
            Spacer sp(31, window_size, v);
            window_size = std::max(window_size, (int)sp.c_);
            Encoder<lex_score> enc(ks->seq.s, ks->seq.l, sp, nullptr);
            uint64_t km, n(0);
            while(enc.has_next_kmer()) {
                ++n;
                if((km = enc.next_minimizer()) != BF)
                    minimizers.push_back(km);
            }
            if(1 /*minimizers.size() != ks->seq.l - window_size - vsum + 1 */) {
                LOG_INFO("Diff: %zd, Expected length: %zu. Found number of minimizers: %zu. Wlen %zu. k: %zu\n",
                         minimizers.size() - (ks->seq.l - window_size + 1),
                         ks->seq.l - window_size + 1, minimizers.size(), (size_t)window_size, 31);
            }
            REQUIRE(minimizers.size() == ks->seq.l - window_size + 1);
            REQUIRE(enc.max_in_queue() == enc.max_in_queue_manual());
            gzclose(fp);
            kseq_destroy(ks);
        }
    }
}