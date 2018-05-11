#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class Point {

	public:
	
	vector <float> points;

	Point(){}

	Point(vector <float> p){
		points = p;
	}

	vector <float> getPoints(){
		return points;
	}

	int getDimension(){
		return points.size();
	}

};

class Cluster{

	public: 
	
	vector <Point> clusterPoints;
    vector <int> :: iterator i;
    Point centroid;

    Cluster(Point cen){
    	centroid = cen;
    }

	Cluster(Point cen, vector <Point> points){
		clusterPoints = points;
		centroid = cen;
	}

	void setCentroid(Point cen){
		centroid = cen;
	}

	Point getCentroid(){
		return centroid;
	}

	void addPoint(Point point){
		clusterPoints.push_back(point);
	}

	vector <Point> getClusterPoints()
	{
		return clusterPoints;
	}

	void clearCluster(){
		clusterPoints.clear();
	}
};

float calculateEucledianDistance(Point a, Point b);
Point calculateCentroid(vector <Point> points);

int main(int argc, char *argv[]){
	
	string file = argv[1];
	int num_clusters = atoi(argv[2]);
	float a, b, c;
	float min_dist, dist;
	int index, end=0;
	vector <Point> points;
	vector <Cluster> clusters, vintage_clusters;

	ifstream infile(file);

	string line;
	for( string line; getline( infile, line ); ){
		istringstream ss(line);
		istream_iterator<float> beg(ss), end;
	    vector<float> tokens(beg, end);
	    points.push_back(tokens);
	}

	for(int i=0; i<num_clusters; i++)
 		clusters.push_back(Cluster(points[i]));

 	while(1){

	 	for(int i=0; i<points.size(); i++){
	 		for(int j=0; j<clusters.size(); j++){
				dist = calculateEucledianDistance(points[i], clusters[j].getCentroid());
				if (j == 0){
					min_dist = dist;
					index = j;
				}else{
					if (min_dist > dist){
						min_dist = dist;
						index = j;
					}
				}


	 		}
	 		clusters[index].addPoint(points[i]);
	 	}

	 	vintage_clusters = clusters;

	 	for(int i=0; i<clusters.size(); i++){

	 		clusters[i].setCentroid(calculateCentroid(clusters[i].getClusterPoints()));

	 		if((vintage_clusters.size() != 0) && (calculateEucledianDistance(vintage_clusters[i].getCentroid(), clusters[i].getCentroid()) == 0)){
	 			end = 1;
	 		}else{
	 			end = 0;
	 		}
	 		
	 		clusters[i].clearCluster();
	 	}

	 	if(end == 1)
	 		break;

	 	vintage_clusters = clusters;
	}
	ofstream output_file("clusters.txt");
    for(int i=0; i<clusters.size(); i++){
    	stringstream ss;
		for(int j = 0; j < clusters[i].getCentroid().getPoints().size(); j++)
		{
		  	if(j != 0){
		    	ss << ' ';
			}
		  	ss << clusters[i].getCentroid().getPoints()[j];
		}
		string s = ss.str();
		output_file << s << "\n";
    }
	return 0;
}

float calculateEucledianDistance(Point a, Point b){

	int d = a.getDimension();
	float dist = 0;
	for(int i=0; i<d; i++){
		dist += pow(a.getPoints()[i] - b.getPoints()[i], 2);
	}
	return sqrt(dist);
}

Point calculateCentroid(vector <Point> points){

	int d = points[0].getDimension();
	vector<float> centroid (d);
	for(int i=0; i<points.size(); i++){
		for (int j=0; j<d; j++){
			centroid[j] += (float) points[i].getPoints()[j] / (float) points.size();
		}
	}
	return Point(centroid);
}
