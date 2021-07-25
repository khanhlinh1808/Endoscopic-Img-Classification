#include <bits/stdc++.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/ml.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"

#define FOR(i, a, b) for(int i = (a), _b = (b); i <= _b ; ++i)
#define For(i, a, b) for(int i = (a), _b = (b); i < _b ; ++i)
#define FORE(i, v) for(__typeof((v).begin()) i = (v).begin() ; i != (v).end() ; ++i)
#define mp make_pair
#define X first
#define Y second

using namespace std;
using namespace cv;

const int N = 1501;

int cnt_train = 0, cnt_test = 0;

class Images {
private:
    vector <float> des;
    int label, size;
public:
    Images(){ des.clear(); label = 0; size = 0;}
    //~Images(){}
    Images (vector <float>& d, int l, int s) {des = d; label = l; size = s; }

    int getLabel(void) { return label; }

    void print()const { 
        For(i, 0, des.size()) printf("%f ", des[i]);
        printf("\n%d %d\n", label, size); 
    }

    float Distance(Images B) {
        float res = 0;
        For(i, 0, B.size) res += (des[i] - B.des[i]) * (des[i] - B.des[i]);
        return sqrt(res);
    }
};

Images Image_train[N], Image_test[N];

void ComputeHOG(string path, int label) {
    HOGDescriptor hog;
    vector <float> descriptor;

    FOR(i, 1, 500) 
    {
        if (label == 1 && i == 491) break;
        
        cv::Mat img = cv::imread(path + "(" + to_string(i) + ").jpg", 0);
        //cout << img.rows << " " << img.cols << endl;
        cv::resize(img, img, Size(64, 128), 0, 0);
        hog.compute(img, descriptor, Size(16, 16), Size(0, 0));
        //cout << descriptor.size() << endl;
        Images A(descriptor, label, descriptor.size());
        if (i <= 425) Image_train[++cnt_train] = A;
        else Image_test[++cnt_test] = A;
        //Image[cnt_train].print();
    }
    /*std::ifstream file("Data/esophagitis/(1).jpg");
    cv::Mat path2;

    while (!file.eof()) {
        file >> path2;
        //cout << 1 << endl;
        cout << path2.rows << endl;
    }*/
}

bool cmp(pair <float, int> a, pair<float, int> b) { return a.first < b.first; }

int Max = 0, id = 0;
float res = 0;
pair <float, int> dis[N];
int cnt[4];

float KNN(int K) {
    printf("%d %d\n", cnt_train, cnt_test);
    res = 0;
    FOR(i, 1, cnt_test) {
        Max = 0;
        FOR(j, 0, cnt_train) dis[j] = mp(0, 0);
        FOR(j, 1, 3) cnt[j] = 0;
        FOR(j, 1, cnt_train) dis[j] = mp(Image_test[i].Distance(Image_train[j]), Image_train[j].getLabel());
        
        sort(dis + 1, dis + cnt_train + 1, cmp);
        //FOR(j, 1, n) cout << dis[j].X << " " << dis[j].Y << endl;
        
        FOR(k, 1, K) ++cnt[dis[k].second];
        //FOR(j, 1, 4) cout << cnt[j] << " "; cout << endl;

        id = 0;
        FOR(j, 1, 3) if (Max <= cnt[j]) Max = cnt[j], id = j;
        //cout << Image_test[i].getLabel() << " " /*<< id*/ << endl;
        res += (cnt[Image_test[i].getLabel()] == Max);

    }
    cout << res << " ";
    return 100.0 * res / cnt_test;
}