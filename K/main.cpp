#include <stdio.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <math.h>

const long double EPS = 1e-10;

class PointInt;
class Point;
class Segment;

class PointInt {
public:
    PointInt() : _x(0), _y(0) {};
    PointInt(const long long & x, const long long & y) : _x(x), _y(y) {};
    PointInt(const PointInt & s) : _x(s.x()), _y(s.y()) {};

    long long x() const
    {
        return _x;
    }

    long long y() const
    {
        return _y;
    }

    void setX(long long newX)
    {
        _x = newX;
    }

    void setY(long long newY)
    {
        _y = newY;
    }

    long long len2()
    {
        return _x * _x + _y * _y;
    }

    PointInt operator+(PointInt & s)
    {
        return PointInt(_x + s.x(), _y + s.y());
    }

    PointInt operator-(PointInt & s)
    {
        return PointInt(_x - s.x(), _y - s.y());
    }

    long long operator^(const PointInt & p)
    {
        return _x * p.y() - _y * p.x();
    }

    long long operator*(const PointInt & p)
    {
        return _x * p.x() + _y * p.y();
    }

    bool operator==(const PointInt & p)
    {
        return (_x == p.x() && _y == p.y());
    }

    friend std::istream& operator>>(std::istream& is, PointInt& p)
    {
        long long x, y;
        is >> x >> y;
        p.setX(x);
        p.setY(y);
        return is;
    }

private:
    long long _x;
    long long _y;
};

class Point {
public:
    Point() : _x(0), _y(0) {};
    Point(const long double & x, const long double & y) : _x(x), _y(y) {};
    Point(const Point & s) : _x(s.x()), _y(s.y()) {};
    Point(const PointInt & s) : _x(s.x()), _y(s.y()) {};

    long double x() const
    {
        return _x;
    }

    long double y() const
    {
        return _y;
    }

    void setX(long double newX)
    {
        _x = newX;
    }

    void setY(long double newY)
    {
        _y = newY;
    }

    long double len()
    {
        return sqrt(long double(_x * _x) + long double(_y * _y));
    }

    Point operator+(const Point & p)
    {
        return Point(_x + p.x(), _y + p.y());
    }

    Point operator-(const Point & p)
    {
        return Point(_x - p.x(), _y - p.y());
    }

    long double operator^(const Point & p)
    {
        return _x * p.y() - _y * p.x();
    }

    long double operator*(const Point & p)
    {
        return _x * p.x() + _y * p.y();
    }

    Point operator*(long double k)
    {
        return Point(k * this->_x, k * this->_y);
    }

    bool operator==(const Point & p)
    {
        return (fabs(_x - p.x()) <= EPS) && (fabs(_y - p.y()) <= EPS);
    }

    bool operator!=(const Point & p)
    {
        return !((*this) == p);
    }

    friend std::istream& operator>>(std::istream& is, Point& p)
    {
        long double x, y;
        is >> x >> y;
        p.setX(x);
        p.setY(y);
        return is;
    }

private:
    long double _x;
    long double _y;
};

class Segment{
public:
    Segment(const PointInt & first, const PointInt & second)
    {
        begin = first;
        end = second;
    }

    PointInt getBegin() const
    {
        return begin;
    }

    PointInt getEnd() const
    {
        return end;
    }

    long double len()
    {
        return sqrt(long double((begin - end).len2()));
    }

private:
    PointInt begin;
    PointInt end;
}; 

long double distance(PointInt a, Segment l)
{
    PointInt b = l.getBegin();
    PointInt c = l.getEnd();
    if ((a - b) * (c - b) <= 0)
        return sqrt(long double((a - b).len2()));
    if ((a - c) * (b - c) <= 0)
        return sqrt(long double((a - c).len2()));
    return fabs(long double((c - a) ^ (b - a))) * 0.5 / l.len();
}

std::pair<Segment, PointInt> findMin(const std::vector<PointInt>& first, const std::vector<PointInt>& second)
{
    int n = first.size();
    int m = second.size();
    int minPos1 = 0, minPos2 = (minPos1 == m - 1 ? 0 : minPos1 + 1);
    Segment currentMin(second[minPos1], second[minPos2]);
    Segment ansMin(second[minPos1], second[minPos2]);
    PointInt minPoint = first[0];
    long double mindist = distance(minPoint, currentMin);
    for (int i = 0; i < n; ++i) {
        long double dist = distance(first[i], currentMin);
        bool min = false;
        do {
            int k1 = (minPos1 == 0 ? m - 1 : minPos1 - 1);
            int k2 = (minPos2 == 0 ? m - 1 : minPos2 - 1);
            int k3 = (minPos1 == m - 1 ? 0 : minPos1 + 1);
            int k4 = (minPos2 == m - 1 ? 0 : minPos2 + 1);
            Segment compare1(second[k1], second[k2]);
            Segment compare2(second[k3], second[k4]);
            long double dist1 = distance(first[i], compare1);
            long double dist2 = distance(first[i], compare2);
            if (dist1 > dist2) {
                compare1 = compare2;
                dist1 = dist2;
                k1 = k3;
                k2 = k4;
            }
            if (dist1 < dist) {
                min = true;
                currentMin = compare1;
                dist = dist1;
                minPos1 = k1;
                minPos2 = k2;
            }
            else
                min = false;
        } while (min);
        if (dist < mindist) {
            mindist = dist;
            minPoint = first[i];
            ansMin = currentMin;
        }
    }
    return std::make_pair(ansMin, minPoint);
}

void twoPoligons()
{
    int n, m;
    std::cin >> n;
    std::vector<PointInt> first, second;
    first.resize(n);
    for (int i = 0; i < n; ++i)
        std::cin >> first[i];
    std::cin >> m;
    second.resize(m);
    for (int i = 0; i < m; ++i)
        std::cin >> second[i];
    std::pair<Segment, PointInt> f = findMin(first, second);
    std::pair<Segment, PointInt> s = findMin(second, first);
    long double dist1 = distance(f.second, f.first);
    long double dist2 = distance(s.second, s.first);
    if (dist1 > dist2) {
        f = s;
        dist1 = dist2;
    }
    PointInt a = f.second;
    PointInt b = f.first.getBegin();
    PointInt c = f.first.getEnd();
    if ((a - b) * (c - b) <= 0) {
        Point mid(long double(a.x() + b.x()) / 2, long double(a.y() + b.y()) / 2);
        long long A = b.x() - a.x();
        long long B = b.y() - a.y();
        long double C = -A * mid.x() - B * mid.y();
        long double norm = sqrt(A * A + B * B);
        std::cout << std::fixed << std::setprecision(15);
        std::cout << long double(A) / norm << ' ' << long double(B) / norm << ' ' << C / norm << std::endl;
        return;
    }
    if ((a - c) * (b - c) <= 0) {
        Point mid(long double(a.x() + c.x()) / 2, long double(a.y() + c.y()) / 2);
        long long A = c.x() - a.x();
        long long B = c.y() - a.y();
        long double C = -A * mid.x() - B * mid.y();
        long double norm = sqrt(A * A + B * B);
        std::cout << std::fixed << std::setprecision(15);
        std::cout << long double(A) / norm << ' ' << long double(B) / norm << ' ' << C / norm << std::endl;
        return;
    }
    /*Line l1(b, c);
    dist1 = l1.distanceToPoint(a);
    */
    Point mid(long double(c.x() + b.x()) / 2, long double(c.y() + b.y()) / 2);
    long long A = -b.y() + c.y();
    long long B = b.x() - c.x();
    long double C = -A * mid.x() - B * mid.y();
    long double dist = (A * a.x() + B * a.y() + C) / sqrt(A * A + B * B);
    /*b = b + move;
    c = c + move;
    long double A = l.getParams().a;
    long double B = l.getParams().b; */
    long double norm = sqrt(A * A + B * B);
    C /= norm;
    C -= (dist * 0.5);
    //long double C = l.getParams().c;
    std::cout << std::fixed << std::setprecision(15);
    std::cout << long double(A) / norm << ' ' << long double(B) / norm << ' ' << C << std::endl;
    return;
}


int main() {
    freopen("rest.in", "r", stdin);
    freopen("rest.out", "w", stdout);
    twoPoligons();
    //system("pause");
    return 0;
}