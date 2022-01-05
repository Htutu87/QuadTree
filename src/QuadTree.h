#ifndef QUADTREE_H
#define QUADTREE_H

#include<vector>
#include<iostream>

class Node;

template <class T>
class QuadTree
{
public:
    class Node
    {
    public:
        Node() {};
        double xmin;
        double xmax;
        double ymin;
        double ymax;
        Node* topLeft;
        Node* topRight;
        Node* bottomLeft;
        Node* bottomRight;
        T element;
    };

public:
    QuadTree() {};
    QuadTree( std::vector<T>& elements );

    Node* root;

public:
    void build( std::vector<T>& elements );

    void split( Node* node, double xmin, double ymin, double xmax, double ymax, std::vector<T>& elements, double minSize = 0.00001 );
    bool intersects( Node* node, double xmin, double ymin, double xmax, double ymax );
    QuadTree<T>::Node* search(Node* node, T point);
    bool inBoundary(Node* node, T point);
    //void select( Node* node, double xmin, double ymin, double xmax, double ymax, std::vector<T>& selectedElements );
};



template <class T>
QuadTree<T>::QuadTree( std::vector<T>& elements )
{
    build( elements );
}



template <class T>
void QuadTree<T>::build( std::vector<T>& elements )
{
    if (elements.size() == 0)
    {
        root = nullptr;
        return;
    }

    double xmin = elements[0].longitude();
    double xmax = elements[0].longitude();
    double ymin = elements[0].latitude();
    double ymax = elements[0].latitude();
    for( auto el : elements)
    {
        xmin = (el.longitude() < xmin)?el.longitude():xmin;
        xmax = (el.longitude() > xmax)?el.longitude():xmax;
        ymin = (el.latitude() < ymin)?el.latitude():ymin;
        ymax = (el.latitude() > ymax)?el.latitude():ymax;
    }

    std::cout << "-------------\nNode geral:\n";
    std::cout << "xmin: " << xmin << "\n";
    std::cout << "xmax: " << xmax << "\n";
    std::cout << "ymin: " << ymin << "\n";
    std::cout << "ymax: " << ymax << "\n";
    std::cout << "-------------\n";

    root = new Node;
    split( root, xmin, ymin, xmax, ymax, elements, 0.0001);
}



template <class T>
void QuadTree<T>::split( Node* node, double xmin, double ymin, double xmax, double ymax, std::vector<T>& elements, double minSize )
{
    /*
    if (elements.size() <= 6)
    {
        std::cerr << "C++: elments.size() = " << elements.size() << std::endl;
        for( auto el : elements )
        {
            std::cerr << "(" << el.x() << "," << el.y() << ") ";
        }
        std::cerr << std::endl;
    }
    */
    node->xmin = xmin;
    node->xmax = xmax;
    node->ymin = ymin;
    node->ymax = ymax;

    node->topLeft = nullptr;
    node->topRight = nullptr;
    node->bottomLeft = nullptr;
    node->bottomRight = nullptr;

    if (elements.size() == 1)
    {
        node->element = elements[0];
    }
    else
    {
        //DEBUG: std::cerr << "{ xmin: " << node->xmin << " ymin: " << node->ymin << " xmax: " << node->xmax << " ymax: " << node->ymax << " }" << std::endl;
        if (((node->xmax - node->xmin) < minSize) || ((node->ymax - node->ymin) < minSize))
            return;

        double xm;
        double ym;

        xm = (node->xmin+node->xmax)/2;
        ym = (node->ymin+node->ymax)/2;

        std::vector<T> topLeftElements;
        std::vector<T> topRightElements;
        std::vector<T> bottomLeftElements;
        std::vector<T> bottomRightElements;
        for (auto el : elements)
        {
            if (el.longitude() <= xm)
            {
                if (el.latitude() <= ym)
                    bottomLeftElements.push_back( el );
                else
                    topLeftElements.push_back( el );
            }
            else
            {
                if (el.latitude() <= ym)
                    bottomRightElements.push_back( el );
                else
                    topRightElements.push_back( el );
            }
        }

        if (topLeftElements.size() > 0)
        {
            node->topLeft = new Node;
            split( node->topLeft, xmin, ym, xm, ymax, topLeftElements, minSize );
        }
        if (topRightElements.size() > 0)
        {
            node->topRight = new Node;
            split( node->topRight, xm, ym, xmax, ymax, topRightElements, minSize );
        }
        if (bottomLeftElements.size() > 0)
        {
            node->bottomLeft = new Node;
            split( node->bottomLeft, xmin, ymin, xm, ym, bottomLeftElements, minSize );
        }
        if (bottomRightElements.size() > 0)
        {
            node->bottomRight = new Node;
            split( node->bottomRight, xm, ymin, xmax, ym, bottomRightElements, minSize );
        }
    }
}

template <class T>

typename QuadTree<T>::Node* QuadTree<T>::search(Node* currentNode, T locomotiveCoordinate)
{
    //Node* nextNode = new Node;


    // Condição de parada: Se o nó atual não possuir nenhum filho, é sinal de que chegamos na "base da árvore".
    if (currentNode->bottomRight == nullptr &&
        currentNode->bottomLeft  == nullptr &&
        currentNode->topRight    == nullptr &&
        currentNode->topRight    == nullptr)
        return currentNode;

    double xm = (currentNode->xmin + currentNode->xmax)/2;
    double ym = (currentNode->ymin + currentNode->ymax)/2;

    // Indica que a locomotiva está em um quadrante superior.
    if (locomotiveCoordinate.latitude() >= ym)
    {
        // Indica que está no quadrante superior direito.
        if (locomotiveCoordinate.longitude() >= xm)
        {
            Node* nextNode(currentNode->topRight);
            return search(nextNode, locomotiveCoordinate);
        }
        // Indica que está no quadrante superior esquerdo
        else
        {
            Node* nextNode(currentNode->topLeft);
            return search(nextNode, locomotiveCoordinate);
        }
    }
    // Indica que a locomotiva está em um quadrante inferior.
    else
    {
        // Indica que está no quadrante inferior direito
        if (locomotiveCoordinate.longitude() >= xm)
        {
            Node* nextNode(currentNode->bottomRight);
            return search(nextNode, locomotiveCoordinate);
        }
        // Indica que está no quadrante inferior esquerdo
        else
        {
            Node* nextNode(currentNode->bottomLeft);
            return search(nextNode, locomotiveCoordinate);
        }
    }
}

// Check if current quadtree contains the point
template <class T>
bool QuadTree<T>::inBoundary(Node* node, T point)
{

}



//void select( Node* node, double xmin, double ymin, double xmax, double ymax, std::vector<T>& selectedElements );


#endif // QUADTREE_H


