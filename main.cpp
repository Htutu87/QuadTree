#include <iostream>
#include "submodules/RailroadTrack.h"
#include "src/QuadTree.h"

using namespace std;
std::vector<QGeoCoordinate> _kmlPoints;

bool loadKmlPoints(RailroadTrack& track)
{
    int time = clock();
    track.readFromFile("/home/developer/workspace/TripAnalyser/submodules/ValeMapView/data/Z23_Eixo_Ferrovia.kml");
    track.readFromFile("/home/developer/workspace/TripAnalyser/submodules/ValeMapView/data/Z24_Eixo_Ferrovia.kml");
    // track.readFromFile("/home/developer/workspace/TripAnalyser/ValeMapView/data/PNs_EFVM.kml");

    for (auto i: track._pathVector)
    {
        int number_points = i.size();

        for(int j = 0; j <= number_points; j++)
        {
            if ( i.coordinateAt(j).isValid() )
                _kmlPoints.push_back(i.coordinateAt(j));
        }
    }
    std::cout << "Quantidade de pontos da ferrovia carregados: " << _kmlPoints.size() << std::endl;

    return true;
}

void nearestTrackPoint( QGeoCoordinate& searchPoint, QGeoCoordinate& nearestPoint, double& nearestDistance )
{
    nearestDistance = 10000000;

    QuadTree<QGeoCoordinate> kmlTree(_kmlPoints);


    QuadTree<QGeoCoordinate>::Node* locomotiveNode = kmlTree.search(kmlTree.root, searchPoint);

//    for(int j = 0 ; j < _kmlPoints.size() ; j++)
//    {
//        double distance = searchPoint.distanceTo( _kmlPoints[j] );
//        if ( distance < nearestDistance )
//        {
//            nearestDistance = distance;
//            nearestPoint = _kmlPoints[j];
//        }
//    }
    std::cout << "Ponto mais proximo: " << nearestDistance << std::endl;
}

int main()
{
    RailroadTrack track;
    loadKmlPoints(track);

    QGeoCoordinate testPoint(-20.330015578,-40.356491974);   //Ponto arbitrário apenas para exemplo

    //QGeoCoordinate testPoint(-20.330015578, -40.356491974);

    QuadTree<QGeoCoordinate> kmlTree(_kmlPoints);

    QuadTree<QGeoCoordinate>::Node* locomotiveNode;

    locomotiveNode = kmlTree.search(kmlTree.root, testPoint);

    std::cout << "-------------\nNode da locomotiva:\n";
    std::cout << "xmin: " << locomotiveNode->xmin << "\n";
    std::cout << "xmax: " << locomotiveNode->xmax << "\n";
    std::cout << "ymin: " << locomotiveNode->ymin << "\n";
    std::cout << "ymax: " << locomotiveNode->ymax << "\n";
    std::cout << "-------------\n";

    std::cout << "locNodeLat: " << locomotiveNode->element.latitude() << "\n";
    std::cout << "locNodeLong: " << locomotiveNode->element.longitude()<< "\n";

    return 0;
}
