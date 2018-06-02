#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <assert.h>
#include <unordered_set>
#include <chrono>
using namespace std::chrono;
using namespace std;
//region defs

class Computation;
void readComputation(Computation& c);
vector<Computation> readComputations();
Computation* readComputationsPtr(int& length);

ostream& writeSpan(ostream& os, const Computation& comp);
ostream& operator<<(std::ostream& os, const Computation& comp);

Computation* mockComputations(int& i);
void printRes(const unordered_set<Computation*> res);

//endregion

// region struct

namespace std {
template<>
struct hash<Computation> {
    size_t operator()(const Computation& c) const noexcept;
};
}

class Computation {
public:
    int start;
    int end;
    unordered_set<Computation*> overlappingComps;

    void addOverlapingComp(Computation* overlap) {
        overlappingComps.insert(overlap);
    }

    size_t overlapNumber()const { return overlappingComps.size(); }
    bool overlapsLeft(const Computation& other)const {
        assert(other.start <= start);
        return other.end >= start;
    }

    bool overlapsRight(const Computation& other) const {
        assert(other.start >= start);
        return end >= other.start;
    }
    Computation& operator=(const Computation& other) = default;

};

size_t std::hash<Computation>::operator()(const Computation& c) const noexcept {
    return (
        (51 + std::hash<int>()(c.start)) * 51
        + std::hash<int>()(c.end)
    );
}

struct StartComparator {
    bool operator()(const Computation& left,
                    const Computation& right) {
        return left.start < right.start;
    }
};
struct OverlapsNumberComparator {
    bool operator()(const Computation& left,
                    const Computation& right) {
        return left.overlapNumber() < right.overlapNumber();
    }
};
//endregion

void computeOverlapingComps(int index, Computation* comp_arr, const int length) {
    Computation& this_comp = comp_arr[index];
    index++;

    while (index < length) {
        auto& other = comp_arr[index];

        if (this_comp.overlapsRight(other)) {
            this_comp.addOverlapingComp(&other);
            other.addOverlapingComp(&this_comp);
            index++;
        }
        else {
            break;
        }
    }
}
//59.974

int main()
{
    auto start = high_resolution_clock::now();
//    auto comps = readComputations();
    int length;
    auto comps = mockComputations(length);
//    auto comps = readComputationsPtr(length);

    sort(comps, comps + length, StartComparator());

     for (int i = 0; i < length; ++i) {
         computeOverlapingComps(i, comps, length);
     }

     sort(comps, comps + length, OverlapsNumberComparator());

    unordered_set<Computation*> result;
    for_each(comps, comps + length, [&result](Computation c) {
        if(! any_of(c.overlappingComps.begin(), c.overlappingComps.end(),
                   [&result](Computation* overlap) {
                    return result.count(overlap) == 0;
        })) {
            result.insert(&c);
        }
    });

    auto finish = high_resolution_clock::now();
    auto seconds = duration_cast<microseconds>(finish - start);
    cerr << "seconds: " << (seconds.count() / 1000.0) << endl;

    ostream_iterator<Computation> output(cerr, ",\n");
    std::copy(comps, comps + length, output);

    printRes(result);

    return 0;
}




void readComputation(Computation& c) {
    int J;
    int D;
    cin >> J >> D;

    c.start = J;
    c.end = J + D;

    cin.ignore();
}

vector<Computation> readComputations() {
    int length;
    cin >> length;
    cin.ignore();
    vector<Computation> comps(length);

    for_each(comps.begin(), comps.end(), readComputation);//[](Computation& comp) { readComputation(comp); });

    return comps;
}

Computation* readComputationsPtr(int& length) {
    cin >> length;
    cin.ignore();
    auto* comps = new Computation[length];

    for (int i = 0; i < length; ++i) {
        readComputation(comps[i]);
    }
    return comps;
}

ostream& writeSpan(ostream& os, const Computation& comp) {
    os << "span: (" << comp.start << ", " << comp.end << ")";
    return os;
}
ostream& operator<<(std::ostream& os, const Computation& comp) {
    os << "{\t";
    writeSpan(os, comp) << "\n" << "\toverlaps: { ";

    vector<Computation*> comp_vec(comp.overlapNumber());
    copy(comp.overlappingComps.begin(), comp.overlappingComps.end(),
         comp_vec.begin());
    sort(comp_vec.begin(), comp_vec.end(), [](
        Computation* left,
        Computation* right) {
        return left->start < right->start;
    });

    for (auto overlapping : comp_vec) {
        writeSpan(os, *overlapping) << ",\n\t\t";
    }
    os << "}\n}";
    return os;
}

Computation* mockComputations(int& i) {
    i = 4;
    return new Computation[4] {
        Computation{ 9, 12 },
        Computation{ 2, 7 },
        Computation{ 15, 21 },
        Computation{ 9, 16 }
    };
}

void printRes(const unordered_set<Computation*> res) {
    cerr << "\nresult: { ";
    for (auto r : res) {
        writeSpan(cerr, *r) << ",\n\t\t";
    }
    cerr << "}\n}";
}