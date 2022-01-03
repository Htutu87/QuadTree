#ifndef RAILROADTRACK_H
#define RAILROADTRACK_H

#include <string>
#include <QXmlDefaultHandler>
#include <QGeoPath>

class RailroadTrack : QXmlDefaultHandler
{
public:
    RailroadTrack();

    bool readFromFile( std::string fileName );

    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &ch);

    bool fatalError(const QXmlParseException &exception);

    int _nPlacemarks;
    int _nCoordinates;

    bool _isReadingCoordinates;

    std::vector<QGeoPath> _pathVector;
};

#endif // RAILROADTRACK_H
