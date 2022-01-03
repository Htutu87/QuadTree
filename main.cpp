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
    kmlTree.build(_kmlPoints);

    for(int j = 0 ; j < _kmlPoints.size() ; j++)
    {
        double distance = searchPoint.distanceTo( _kmlPoints[j] );
        if ( distance < nearestDistance )
        {
            nearestDistance = distance;
            nearestPoint = _kmlPoints[j];
        }
    }
    std::cout << "Ponto mais proximo: " << nearestDistance << std::endl;
}

int main()
{
    RailroadTrack track;
    loadKmlPoints(track);

    QGeoCoordinate testPoint(-19.9098, -43.9127);   //Ponto arbitrário apenas para exemplo
    QGeoCoordinate nearestPoint;
    double minDistanceToTrack;

    // Procurando o ponto mais proximo da Ferrovia
    nearestTrackPoint( testPoint, nearestPoint, minDistanceToTrack );

    return 0;
}
