#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;
// region struct

class Computation {
public:
    int start;
    int end;
    vector<Computation> overlapingComps;

    Computation& operator=(const Computation& other) = default;

    size_t overlapNumber() { return overlapingComps.size(); }
    void computeOverlapingComps() {

    }
    bool overlaps(const Computation& other) {
        return other.end > start ||
               other.start < end;
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

    for (auto& overlapping : comp.overlapingComps) {
        writeSpan(os, overlapping) << ",\n\t\t\t";
    }
    os << "}\n}";
    return os;
}


struct StartComparator {
    bool operator() (const Computation& left, const Computation& right) {
        return left.start < right.start;
    }
};//endregion

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

    for_each(comps.begin(), comps.end(),
             [](Computation& comp) { comp.computeOverlapingComps(); });
    return comps;
}

int main()
{
//    vector<Computation> comps = readComputations();
//
//    sort(comps.begin(), comps.end(), StartComparator());
//
//    for_each(comps.begin(), comps.end(), [](const Computation& comp) {
//
//    });
//
//    ostream_iterator<Computation> output(cerr, " ");
//    std::copy(comps.begin(), comps.end(), output);

    Computation comp;

    comp.overlapingComps.push_back(Computation{ 4, 4});
    comp.overlapingComps.push_back(Computation{ 4, 4 });


    cout << comp;

    return 0;
}