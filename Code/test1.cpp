#include "Header.h"

int main()
{
    string path[4] = { "", "Data/esophagitis/", "Data/polyps/", "Data/ulcerative-colitis/"};
    
    /*FOR(i, 1, 4) ComputeHOG(path[i], i);
    cout << KNN(8) << endl;
    return 0;*/
    FOR(i, 1, 3) ComputeHOG(path[i], i);
    
    printf("Accuracy = %f\n", KNN(2));
    
    return 0;
}