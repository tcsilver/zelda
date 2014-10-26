#include "path.hpp"

const int Path::RESOLUTION = 1;

Path::Path(Mob* from, Entity* to) :
        from(from),
        to(to),
        rectangle(new Rectangle(0, 0, from->width(), from->height())),
        ready(false),
        calculating(false),
        accum(0),
        found(false)
{
    origin = vec2i((int)(from->x() / Path::RESOLUTION), (int)(from->y() / Path::RESOLUTION));
    destination = vec2i((int)(to->x() / RESOLUTION), (int)(to->y() / RESOLUTION));
}

Path::~Path() {
    delete rectangle;
}

bool Path::Update(double delta) {
    accum += delta;

    if(found) {
        if(accum < 0.2)
            return true;

        const vec2i& last = nodes[0];

        rectangle->set_position(last.x * RESOLUTION, last.y * RESOLUTION);
        return rectangle->CollidesWith(to);
    }

    return accum < 2;
}

Path::Node::Node(const vec2i& position, const vec2i& destination, unsigned int g_cost, Path::Node* parent) :
        x((unsigned int) position.x),
        y((unsigned int) position.y),
        g_cost(g_cost),
        parent(parent),
        closed(false)
{
    h_cost = HeuristicCost(destination);
    cost = g_cost + h_cost;
}

bool Path::Node::SortByCostAsc::operator()(Path::Node* n1, Path::Node* n2) const {
    return n1->cost < n2->cost or (n1->cost == n2->cost and n1->x < n2->x) or (n1->x == n2->x and n1->y < n2->y);
}

unsigned int Path::Node::HeuristicCost(const vec2i& destination) const {
    return (unsigned int)(abs(destination.x - x) + abs(destination.y - y));
}

void Path::Node::UpdateGCost(unsigned int g_cost) {
    this->g_cost = g_cost;
    cost = g_cost + h_cost;
}

void Path::Render() const {
    if(ready) {
        for(const vec2i node : nodes) {
            Rectangle r(node.x * RESOLUTION, node.y * RESOLUTION, RESOLUTION, RESOLUTION);
            r.Render(1, 1, 1);
        }
    }
}
