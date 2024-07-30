#include "header.h"

map<__uint128_t, unsigned int> build_pdata(vector<pair<__uint128_t, unsigned int>> &data, __uint128_t community) {

    map<__uint128_t, unsigned int> pdata;
    __uint128_t mask_state;

    for (const auto &[state, count] : data) {
        mask_state = state & community;
        pdata[mask_state] += count;
    }

    return pdata;
}

double icc_evidence(__uint128_t community, Partition &p_struct){

    double logE = 0;
    double pf;
    
    unsigned int rank = bit_count(community);
    
    if (rank > 32) {
        pf = -((double) rank - 1.) * p_struct.N * LOG2;
    } else {
        double rank_pow = (double) (ONE << (rank - 1));
        pf = lgamma(rank_pow) - lgamma(p_struct.N + rank_pow);
    }

    logE += pf;

    map<__uint128_t, unsigned int> pdata = build_pdata(p_struct.data, community);

    for (const auto &pstate : pdata) {
        const unsigned int &count = pstate.second;
        if (count < 10) {
            logE += LGAMMA[count] - SQRT_PI;
        } else {
            logE += lgamma(count + 0.5) - SQRT_PI;
        }      
    }

    return logE;
}

double get_evidence(__uint128_t community, Partition &p_struct) {

    double evidence;

    auto check = p_struct.evidence_memo.find(community);
    if (check != p_struct.evidence_memo.end()) {evidence = check->second;}
    else {
        evidence = icc_evidence(community, p_struct);
        p_struct.evidence_memo.emplace(community, evidence);
        }
    return evidence;
}