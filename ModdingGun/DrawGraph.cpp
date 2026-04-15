#include "DrawGraph.h"
#include "LangManager.h"
#include "ThreadAssemble.h"


CurvePack GetCurveFrequrency
(
    RectangleShape mainRectangleCurve,
    const map<CountModifiers, float>& freqModifiers,
    Color colorCurve,
    string name
)
{
    float kX = 0.2f;
    float kY = 0.1f;

    float xPosGeneral = mainRectangleCurve.getPosition().x;
    float yPosGeneral = mainRectangleCurve.getPosition().y;

    float xSizeRecText = mainRectangleCurve.getSize().x * kX;
    float ySizeRecText = mainRectangleCurve.getSize().y * kY;

    float yPrecenSizeCell = 0.9f;

    int characterSize = static_cast<int>(14 * yPrecenSizeCell);
    int yPositionInfoText = 0;

    RectangleShape r0InfoLeftTop = mainRectangleCurve;
    r0InfoLeftTop.setPosition(Vector2f(xPosGeneral - xSizeRecText, yPosGeneral- ySizeRecText * (1 + yPositionInfoText)));
    r0InfoLeftTop.setSize(Vector2f(xSizeRecText + mainRectangleCurve.getSize().x, ySizeRecText * yPrecenSizeCell));

    RectangleShape r1LeftTop = mainRectangleCurve;
    r1LeftTop.setPosition(Vector2f(xPosGeneral - xSizeRecText, mainRectangleCurve.getPosition().y));
    r1LeftTop.setSize(Vector2f(xSizeRecText, ySizeRecText * yPrecenSizeCell));

    RectangleShape r1LeftDown = mainRectangleCurve;
    r1LeftDown.setPosition(Vector2f(xPosGeneral - xSizeRecText, yPosGeneral+ mainRectangleCurve.getSize().y - ySizeRecText));
    r1LeftDown.setSize(Vector2f(xSizeRecText, ySizeRecText * yPrecenSizeCell));

    RectangleShape r2LeftDown = mainRectangleCurve;
    r2LeftDown.setPosition(Vector2f(xPosGeneral - xSizeRecText, yPosGeneral+ mainRectangleCurve.getSize().y));
    r2LeftDown.setSize(Vector2f(xSizeRecText, ySizeRecText * yPrecenSizeCell));

    RectangleShape r2RightDown = mainRectangleCurve; 
    r2RightDown.setPosition(Vector2f(xPosGeneral + mainRectangleCurve.getSize().x, yPosGeneral+ mainRectangleCurve.getSize().y));
    r2RightDown.setSize(Vector2f(xSizeRecText, ySizeRecText * yPrecenSizeCell));

    vector< GUITextAndRectangle> m_vGUITextCurve =
    {
        {{name}, r0InfoLeftTop ,false, false},
        {{"Often"}, r1LeftTop      ,false, false},
        {{"Rarely"}, r1LeftDown     ,false, false},
        {{"CountMin"}, r2LeftDown     ,false, false},
        {{"CountMax"}, r2RightDown    ,false, false},
    };

    m_vGUITextCurve[0].setColorText(colorCurve);

    if (freqModifiers.empty())
    {
        OUTPUT_LOG("GetCurveForModifiers -> freqModifiers empty");
        m_vGUITextCurve[0].setKeyString_and_TextWstring({ "empty" });

        return { mainRectangleCurve, m_vGUITextCurve, VertexArray() };
    }

    return { mainRectangleCurve, m_vGUITextCurve,  CurveFun(mainRectangleCurve, colorCurve, freqModifiers)};
}

VertexArray CurveFun(RectangleShape mainRectangleCurve, Color ColorCurve, const map<CountModifiers, float>& FreqModifiers)
{

    float xSize = mainRectangleCurve.getSize().x;
    float ySize = mainRectangleCurve.getSize().y;
    float xPos = mainRectangleCurve.getPosition().x;;
    float yPos = mainRectangleCurve.getPosition().y;

    VertexArray curve(PrimitiveType::LineStrip, FreqModifiers.size());

    // 1. Найдём максимальную частоту
    float minFreq = FLT_MAX;
    float maxFreq = -FLT_MAX;

    for (const auto& [_, freq] : FreqModifiers)
    {
        minFreq = min(minFreq, freq);
        maxFreq = max(maxFreq, freq);
    }

    // 2. Генерируем график
    int i = 0;
    float dx = xSize / max(1.0f, static_cast<float>(FreqModifiers.size() - 1));

    for (auto& [_, freq_count] : FreqModifiers)
    {
        float x = xPos + dx * i;

        // Нормализуем: 1 -> нижняя граница, max_freq -> верхняя граница
        float range = max(0.0001f, maxFreq - minFreq); // защита от деления на 0
        float t = (freq_count - minFreq) / range;
        float y = yPos + ySize * (1.f - t);

        curve[i].position = Vector2f(x, y);
        curve[i].color = ColorCurve;

        i++;
    }

    return curve;
}

map<CountModifiers, float> GetFreqModifiers(const vector<CountModifiers>& otherVectorCountModifiers)
{
    map<CountModifiers, float> result;

    if (otherVectorCountModifiers.empty())
    {
        OUTPUT_LOG("GetFreqCountModifiers -> empty");
    }

    for (size_t i = 0; i < otherVectorCountModifiers.size(); i++)
    {
        result[otherVectorCountModifiers[i]]++;
    }
    return result;
}

map<CountModifiers, float> GetBinedFreqModifiers(const map<CountModifiers, float>& otherMap, float percentBin)
{
    if (otherMap.empty())
    {
        return {};
    }

    // Если указано 100%, биннинг не нужен
    if (percentBin >= 1.0f)
    {
        return otherMap;
    }
    
    // Определение количества корзин
    int countBascet = max(1, static_cast<int>(otherMap.size() * percentBin));
    
    // Сбор и сортировка всех частот с привязкой к CountModifiers
    vector<pair<CountModifiers, float>> sortedFreqs(otherMap.begin(), otherMap.end());

    sort(sortedFreqs.begin(), sortedFreqs.end(),
        [](const auto& a, const auto& b)
        {
            return a.second < b.second;
        });
    
    // Распределение по корзинам по квантилям
    map<int, vector<pair<CountModifiers, float>>> bins;

    int total = static_cast<int>(sortedFreqs.size());
    int itemsPerBin = total / countBascet;

    int currentBin = 0;
    int countInCurrent = 0;

    for (const auto& [mod, freq] : sortedFreqs)
    {
        bins[currentBin].push_back({ mod, freq });
        countInCurrent++;

        if (countInCurrent >= itemsPerBin && currentBin + 1 < countBascet)
        {
            currentBin++;
            countInCurrent = 0;
        }
    }
    
    
    // Формирование результата на основе среднего значения в корзине
    map<CountModifiers, float> result;

    for (const auto& [binIdx, items] : bins)
    {
        if (items.empty())
        {
            continue;
        }

        CountModifiers representative = items[0].first;

        float sumFreq = 0.0f;
        for (const auto& [_, freq] : items)
        {
            sumFreq += freq;
        }

        float averageFreq = sumFreq / items.size();

        result[representative] = averageFreq;
    }

    return result;
}

map<CountModifiers, float> GetSmoothFreqModifiers(const map<CountModifiers, float>& otherMap, float smoothnessPercent)
{
    map<CountModifiers, float> result;

    if (otherMap.empty())
    {
        OUTPUT_LOG("GetFreqCountModifiers -> empty");
        return result;
    }

    if (smoothnessPercent == 0)
    {
        return otherMap;
    }
    
    int totalSize = static_cast<int>(otherMap.size());
    float radius = max(0, smoothnessPercent * totalSize);

    vector<pair<CountModifiers, float>> items(otherMap.begin(), otherMap.end());

    for (int i = 0; i < totalSize; ++i)
    {
        float sum = 0.0f;
        int count = 0;

        for (size_t j = static_cast<size_t>(max(0, i - radius)); j <= static_cast<size_t>(min(totalSize - 1, i + radius)); ++j)
        {
            sum += items[j].second;
            count++;
        }

        result[items[i].first] = sum / count;
    }

    return result;
}

CountModifiers GetPercentile(const map<CountModifiers, float>& freqMap, float percentile)
{
    float total = 0.0f;

    for (const auto& [_, Freq] : freqMap)
    {
        total += Freq;
    }

    float threshold = ceil(percentile / 100.0f * total);
    float cumulative = 0.0f;

    for (const auto& [value, Freq] : freqMap)
    {
        cumulative += Freq;

        if (cumulative >= threshold)
        {
            return value;
        }
    }

    // если не нашли — вернём последний ключ
    return freqMap.rbegin()->first;
}
