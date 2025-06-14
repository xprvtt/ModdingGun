#include "Core.h"

double getChance(int position, const vector<pair<int, double>>& chancePoints)
{

    for (size_t i = 0; i < chancePoints.size() - 1; ++i)
    {
        int x1 = chancePoints[i].first;
        double y1 = chancePoints[i].second;
        int x2 = chancePoints[i + 1].first;
        double y2 = chancePoints[i + 1].second;

        if (position >= x1 && position <= x2)
        {
            double ratio = static_cast<double>(position - x1) / (x2 - x1);
            return y1 + (y2 - y1) * ratio;
        }
    }

    return 0.0; // если позиция вне диапазона
}
