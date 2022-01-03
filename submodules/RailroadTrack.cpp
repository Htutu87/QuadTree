#include "RailroadTrack.h"

#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QFile>
#include <QStringList>
#include <QDebug>

RailroadTrack::RailroadTrack()
{

}



bool RailroadTrack::readFromFile( std::string fileName )
{
    QFile file( fileName.c_str() );

    QXmlSimpleReader xmlReader;
    QXmlInputSource* source = new QXmlInputSource( &file );
    //std::string queryString = "doc('" + fileName + "')/kml/Document/Folder/Placemark/LineString/coordinates";
    //qDebug() << queryString.c_str();

    //Handler *handler = new Handler;
    xmlReader.setContentHandler( this );
    xmlReader.setErrorHandler( this );

    _nPlacemarks = 0;
    _nCoordinates = 0;
    _isReadingCoordinates = false;

    bool ok = xmlReader.parse( source );

    if (!ok)
      qDebug() << "Parsing failed.";

    qDebug() << "Placemarks = " << _nPlacemarks;
    qDebug() << "Coordinates = " << _nCoordinates;

    delete source;

    return true;
}




bool RailroadTrack::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    //qDebug() << "namespaceURI = " << namespaceURI;
    //qDebug() << "localName = " << localName;
    //qDebug() << "qName = " << qName;
    if (localName == "Placemark")
        _nPlacemarks++;
    if (localName == "coordinates")
    {
        _pathVector.push_back( QGeoPath() );
        _isReadingCoordinates = true;
        _nCoordinates++;
    }
    return true;
}



bool RailroadTrack::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    if (localName == "coordinates")
        _isReadingCoordinates = false;

    return true;
}



bool RailroadTrack::characters(const QString &ch)
{
    //qDebug() << "\t" << ch;
    if (_isReadingCoordinates)
    {
        QStringList strList = ch.split( " ", QString::SkipEmptyParts );
        //qDebug() << ch;
        for (QString str : strList)
        {
            //qDebug() << str;
            QStringList coords = str.split( ",", QString::SkipEmptyParts );
            if (coords.size() >= 2)
            {
                int i=0;
                while (coords[0][i] == '\\' || coords[0][i] == '\n' || coords[0][i] == '\t' )
                {
                    coords[0][i] = ' ';
                    i++;
                }
                double latitude  = coords[1].toDouble();
                double longitude = coords[0].toDouble();
                //qDebug() << coords[0] << " " << coords[1];
                _pathVector[_pathVector.size()-1].addCoordinate( QGeoCoordinate(latitude, longitude));
            }
        }
    }

    return true;
}



bool RailroadTrack::fatalError(const QXmlParseException &exception)
{
    qDebug() << "fatalError: " + exception.message();
}



