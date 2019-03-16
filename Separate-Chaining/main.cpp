
#include <iostream>
#include <vector>
#include "ADS_set.h"

using namespace std;

int main(int argc, const char * argv[]) {
 
    ADS_set<int> ads {1,2,34,1,2,8};
    ads.dump();
    
    for(auto i: ads)
        cout << i << endl;
    
    return 0;
}
