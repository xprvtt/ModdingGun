#include "Core.h"

double getChance(int position, const vector<pair<int, double>>& chancePoints)
{

    for (size_t i = 0; i < chancePoints.size() - 1; ++i)
    {
        auto point1 = chancePoints[i];
        auto point2 = chancePoints[i + 1];

        int x1 = point1.first;
        double y1 = point1.second;

        int x2 = point2.first;
        double y2 = point2.second;

        if (position >= x1 && position <= x2)
        {
            double ratio = static_cast<double>(position - x1) / (x2 - x1);
            return y1 + (y2 - y1) * ratio;
        }
    }

    return 0.0; // если позиция вне диапазона
}

