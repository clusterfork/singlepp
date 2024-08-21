#ifndef SINGLEPP_INTERSECTION_HPP
#define SINGLEPP_INTERSECTION_HPP

#include "macros.hpp"

#include <vector>
#include <algorithm>
#include <cstdint>
#include <numeric>
#include <unordered_map>

/**
 * @file Intersection.hpp
 * @brief Intersection of features.
 */

namespace singlepp {

/**
 * Intersection of features between two datasets (typically test and reference).
 * Each element corresponds to a pair of matching features and contains the row indices of those features in the test (`first`) or reference (`second`) dataset.
 */
template<typename Index_>
using Intersection = std::vector<std::pair<Index_, Index_> >;

/**
 * Compute the intersection of genes in the test and reference datasets.
 *
 * @tparam Index_ Integer type for the row indices of genes in each dataaset.
 * Also used as the type for the number of genes.
 * @tparam Id_ Type of the gene identifier, typically an integer or string.
 *
 * @param test_ngenes Number of genes (i.e., rows) in the test dataset.
 * @param[in] test_id Pointer to an array of length `test_ngenes`, containing the gene identifiers for each row in the test dataset.
 * @param ref_ngenes Number of genes (i.e., rows) in the reference dataset.
 * @param[in] ref_id Pointer to an array of length `ref_ngenes`, containing the gene identifiers for each row in the reference dataset.
 * 
 * @return Intersection of features between the two datasets.
 * If duplicated identifiers are present in either of `test_id` or `ref_id`, only the first occurrence is used.
 */
template<typename Index_, typename Id_>
Intersection<Index_> intersect_genes(Index_ test_ngenes, const Id_* test_id, Index_ ref_ngenes, const Id_* ref_id) {
    std::unordered_map<Id_, Index_> ref_found;
    for (Index_ i = 0; i < ref_ngenes; ++i) {
        auto current = ref_id[i];
        auto tfIt = ref_found.find(current);
        if (tfIt == ref_found.end()) { // only using the first occurrence of each ID in ref_id.
            ref_found[current] = i;
        }
    }

    Intersection<Index_> output;
    for (Index_ i = 0; i < test_ngenes; ++i) {
        auto current = test_id[i];
        auto tfIt = ref_found.find(current);
        if (tfIt != ref_found.end()) {
            output.emplace_back(i, tfIt->second);
            ref_found.erase(tfIt); // only using the first occurrence of each ID in test_id; the next will not enter this clause.
        }
    }

    // This is implicitly sorted by the test indices... not that it really
    // matters, as subset_to_markers() doesn't care that it's unsorted.
    return output;
}

}

#endif
