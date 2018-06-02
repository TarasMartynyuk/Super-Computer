#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <assert.h>
#include <unordered_set>
using namespace std;
// region struct

class Computation;
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

ostream& writeSpan(ostream& os,
                   const Computation& comp) {
    os << "span: (" << comp.start << ", " << comp.end << ")";
    return os;
}
std::ostream& operator<<(std::ostream& os, const Computation& comp) {
    os << "{\t";
    writeSpan(os, comp) << "\n" << "\toverlaps: { ";

    vector<Computation*> comp_vec(comp.overlapNumber());
    copy(comp.overlappingComps.begin(), comp.overlappingComps.end(),
         comp_vec.begin());
    sort(comp_vec.begin(), comp_vec.end(), [](
        Computation* left, Computation* right) {
        return left->start < right->start;
    });

    for (auto overlapping : comp_vec) {
        writeSpan(os, *overlapping) << ",\n\t\t";
    }
    os << "}\n}";
    return os;
}//endregion
//region funcs

void readComputation(Computation& c) {
    int J;
    int D;
    cin >> J >> D;

    c.start = J;
    c.end = J + D;

    cin.ignore();
}
vector<Computation> readComputations() {
    int length;     cin >> length;    cin.ignore();
    vector<Computation> comps(length);

    for_each(comps.begin(), comps.end(), readComputation);//[](Computation& comp) { readComputation(comp); });
    return comps;
}

void computeOverlapingComps(vector<Computation>& comp_vec, int index) {
    Computation& this_comp = comp_vec.at(index);
    index++;

    while (index < comp_vec.size()) {
        auto& other = comp_vec.at(index);

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
//endregion

vector<Computation> mockComputations() {
    return vector<Computation> {
        Computation{ 9, 12 },
        Computation{ 2, 7 },
        Computation{ 15, 21 },
        Computation{ 9, 16 }
    };
}

int main()
{
    // auto comps = mockComputations();
    vector<Computation> comps = readComputations();
//
    sort(comps.begin(), comps.end(), StartComparator());

    for (int i = 0; i < comps.size(); ++i) {
        computeOverlapingComps(comps, i);
    }

    ostream_iterator<Computation> output(cerr, ",\n");
    std::copy(comps.begin(), comps.end(), output);
//
    return 0;
}