/// DeletableBloomFilter implements a Deletable Bloom Filter as described by
/// Rothenberg, Macapuna, Verdi, Magalhaes in The Deletable Bloom filter - A new
/// member of the Bloom family:
///
/// http://arxiv.org/pdf/1005.0352.pdf
///
/// The code has been ported from https://github.com/mattlorimor/ProbabilisticDataStructures/blob/master/ProbabilisticDataStructures/DeletableBloomFilter.cs

#include "del-bf.h"

DeletableBloomFilter::DeletableBloomFilter(uint n, uint r, double fpRate){
    uint optM = optimalM(n, fpRate);
    uint optK = optimalK(fpRate);

    buckets = std::vector<bool>(optM - r);
    collisions = std::vector<bool>(r);
    m = optM - r;
    regionSize = (optM - r) / r;
    k = optK;
}

/// <summary>
/// Returns the number of items added to the filter.
/// </summary>
/// <returns>The number of items added to the filter</returns>
uint DeletableBloomFilter::getCount(){
    return count;
}

/// <summary>
/// Will test for membership of the data and returns true if it is a member,
/// false if not. This is a probabilistic test, meaning there is a non-zero
/// probability of false positives but a zero probability of false negatives.
/// </summary>
/// <param name="data">The data to search for.</param>
/// <returns>Whether or not the data is maybe contained in the filter.</returns>
bool DeletableBloomFilter::test(char* data, int len){
    // If any of the K bits are not set, then it's not a member.
    uint32_t hash;
    for (uint i = 0; i < k; i++){
        MurmurHash3_x86_32(data, len, i, (void*) &hash);
        hash %= m;
        if (buckets[hash] == 0){
            return false;
        }
    }
    return true;
}

/// <summary>
/// Will add the data to the Bloom filter.
/// </summary>
/// <param name="data">The data to add.</param>
void DeletableBloomFilter::add(char* data, int len){
    uint32_t hash;
    // Set the K bits.
    for (uint i = 0; i < k; i++){
        MurmurHash3_x86_32(data, len, i, (void*) &hash);
        hash %= m;
        if (buckets[hash] != 0){
            // Collision, set corresponding region bit.
            collisions[hash / regionSize] = 1;
        }else{
            buckets[hash] = 1;
        }
    }
    count++;
}

/// <summary>
/// Is equivalent to calling Test followed by Add. It returns true if the data is
/// a member, false if not.
/// </summary>
/// <param name="data">The data to test for and add if it doesn't exist.</param>
/// <returns>Whether or not the data was probably contained in the filter.</returns>
bool DeletableBloomFilter::testAndAdd(char* data, int len){
    bool member = true;
    uint32_t hash;
    // If any of the K bits are not set, then it's not a member.
    for (uint i = 0; i < k; i++){
        MurmurHash3_x86_32(data, len, i, (void*) &hash);
        hash %= m;
        if (buckets[hash] == 0){
            member = false;
        }else{
            // Collision, set corresponding region bit.
            collisions[hash / regionSize] = 1;
        }
        buckets[hash] = 1;
    }
    count++;
    return member;
}

/// <summary>
/// Will test for membership of the data and remove it from the filter if it
/// exists. Returns true if the data was a member, false if not.
/// </summary>
/// <param name="data">The data to test for and remove</param>
/// <returns>Whether or not the data was a member before this call</returns>
bool DeletableBloomFilter::testAndRemove(char* data, int len){
    bool member = true;
    uint32_t hash;
    // Set the K bits.
    for (uint i = 0; i < k; i++){
        MurmurHash3_x86_32(data, len, i, (void*) &hash);
        hash %= m;
        if (buckets[hash] == 0){
            member = false;
        }
    }

    if (member){
        for (uint i = 0; i < k; i++){
            MurmurHash3_x86_32(data, len, i, (void*) &hash);
            hash %= m;
            if (collisions[hash / regionSize] == 0){
                // Clear only bits located in collision-free zones.
                buckets[hash] = 0;
            }
        }
        count--;
    }

    return member;
}

/// <summary>
/// Restores the Bloom filter to its original state. 
/// </summary>
void DeletableBloomFilter::reset(){
    buckets.assign(buckets.size(), 0);
    collisions.assign(collisions.size(), 0);
    count = 0;
}
