/// DeletableBloomFilter implements a Deletable Bloom Filter as described by
/// Rothenberg, Macapuna, Verdi, Magalhaes in The Deletable Bloom filter - A new
/// member of the Bloom family:
///
/// http://arxiv.org/pdf/1005.0352.pdf
///
/// The code has been ported from https://github.com/mattlorimor/ProbabilisticDataStructures/blob/master/ProbabilisticDataStructures/DeletableBloomFilter.cs

#include "hash.h"

#include <cmath>
#include <vector>

#define FILL_RATIO (0.5)

class DeletableBloomFilter{
private:
    std::vector<bool> buckets; /// Filter data
    std::vector<bool> collisions; /// Filter collision data
    uint m; /// Filter size
    uint regionSize; /// Number of bits in a region
    uint k; /// Number of hash functions
    uint count; /// Number of items in the filter

    static uint optimalM(uint n, double fpRate){
        return std::ceil((double) n / ((std::log(FILL_RATIO) *
                std::log(1 - FILL_RATIO)) / std::abs(std::log(fpRate))));
    }

    static uint optimalK(double fpRate){
        return std::ceil(std::log2(1 / fpRate));
    }

public:
    /// <summary>
    /// NewDeletableBloomFilter creates a new DeletableBloomFilter optimized to store
    /// n items with a specified target false-positive rate. The r value determines
    /// the number of bits to use to store collision information. This controls the
    /// deletability of an element. Refer to the paper for selecting an optimal value.
    /// </summary>
    /// <param name="n">Number of items</param>
    /// <param name="r">Number of bits to use to store collision information</param>
    /// <param name="fpRate">Desired false positive rate</param>
    DeletableBloomFilter(uint n, uint r, double fpRate);

    /// <summary>
    /// Returns the number of items added to the filter.
    /// </summary>
    /// <returns>The number of items added to the filter</returns>
    uint getCount();

    /// <summary>
    /// Will test for membership of the data and returns true if it is a member,
    /// false if not. This is a probabilistic test, meaning there is a non-zero
    /// probability of false positives but a zero probability of false negatives.
    /// </summary>
    /// <param name="data">The data to search for.</param>
    /// <returns>Whether or not the data is maybe contained in the filter.</returns>
    bool test(char* data, int len);

    /// <summary>
    /// Will add the data to the Bloom filter.
    /// </summary>
    /// <param name="data">The data to add.</param>
    void add(char* data, int len);

    /// <summary>
    /// Is equivalent to calling Test followed by Add. It returns true if the data is
    /// a member, false if not.
    /// </summary>
    /// <param name="data">The data to test for and add if it doesn't exist.</param>
    /// <returns>Whether or not the data was probably contained in the filter.</returns>
    bool testAndAdd(char* data, int len);

    /// <summary>
    /// Will test for membership of the data and remove it from the filter if it
    /// exists. Returns true if the data was a member, false if not.
    /// </summary>
    /// <param name="data">The data to test for and remove</param>
    /// <returns>Whether or not the data was a member before this call</returns>
    bool testAndRemove(char* data, int len);

    /// <summary>
    /// Restores the Bloom filter to its original state. 
    /// </summary>
    void reset();
};
