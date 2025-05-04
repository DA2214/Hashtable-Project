#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>   

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        unsigned long long w[5] = {0,0,0,0,0};
        int len = static_cast<int>(k.size());
        int wi  = 4;

        for(int end = len; end > 0 && wi >= 0; end -= 6, --wi) {
            int start = std::max(0, end - 6);
            w[wi] = chunkValue(k, start, end);
        }

        unsigned long long h = 0ULL;
        for(int i = 0; i < 5; ++i) {
            h += static_cast<unsigned long long>(rValues[i]) * w[i];
        }
        return h;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char c) const
    {
        if(c >= 'A' && c <= 'Z')
            c = static_cast<char>(c - 'A' + 'a');

        if(c >= 'a' && c <= 'z')  
            return static_cast<HASH_INDEX_T>(c - 'a');
        else  
            return static_cast<HASH_INDEX_T>(c - '0' + 26);
    }

private:
    unsigned long long chunkValue(const std::string& k, int start, int end) const {
        unsigned long long val = 0;
        unsigned long long base = 36;
        for(int i = start; i < end; ++i) {
            val = val * base + letterDigitToNumber(k[i]);
        }
        return val;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        unsigned seed = std::chrono::system_clock::now()
                            .time_since_epoch().count();
        std::mt19937 generator(seed);
        for(int i = 0; i < 5; ++i) {
            rValues[i] = generator();
        }
    }
};

#endif
