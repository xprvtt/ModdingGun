#include "Core.h"


int percentile(const vector<float>& data, double p) 
{
    if (data.empty()) return 0;

    vector<float> sorted = data;
    std::sort(sorted.begin(), sorted.end());

    double pos = p * (sorted.size() - 1); // позиция
    size_t idx = static_cast<size_t>(pos);
    double frac = pos - idx;

    // линейная интерполяция, если pos не целое
    if (idx + 1 < sorted.size()) {
        return sorted[idx] * (1 - frac) + sorted[idx + 1] * frac;
    }
    else {
        return sorted[idx];
    }
}








// ДЕБРИ ////
// ДЕБРИ ////
// ДЕБРИ ////
// ДЕБРИ ////
// ДЕБРИ ////
// ДЕБРИ ////
vector<int> drawNormalGraph(
    RenderWindow& window,
    const vector<float>& Average10000,
    float graphHeight,
    float marginBottom,
    const vector<float>& Average10000_RANDOM_GEN_SOURCE,
    int   totalIterations,
    const vector<double>& RESULT_CHARACTERISTIC,
    const vector<double>& START_CHARACTERISTIC,
    const vector<double>& INITIAL_CHARACTERISTIC,
    wstring NameGun,
    vector<int> method,
    const tuple<double, double, double>& Tool_Kit_Skil
)
{
    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------

    vector<float> Average10000_RANDOM_GEN;


    if (Average10000.empty())
    {
        OutputLog("Average10000.empty()");
        return {};
    }
    if (Average10000_RANDOM_GEN_SOURCE.empty())
    {
        OutputLog("Average10000_RANDOM_GEN.empty()");
        Average10000_RANDOM_GEN.push_back(0.f);
    }

    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------























    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------

    // задний фон



    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Assets/Standart/background_graf.png"))
    {
        // Обработка ошибки
        OutputLog("Не удалось загрузить background_graf.png");
    }
    else
    {
        RectangleShape background;
        background.setSize(Vector2f(window.getSize().x, window.getSize().y));
        background.setPosition(Vector2f(0, 0));
        background.setTexture(&backgroundTexture, true);
        window.draw(background);
    }


    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------

    vector<path> SearchFont = SearchFile("Font/", ".ttf");
    if (SearchFont.empty())
    {
        OutputLog("Шрифт не найден 2, завершение");
        return {};
    }
    OutputLog("попытка");
    Font font = LoadFont(SearchFont[0]);
    OutputLog("загружен");


    const auto COLOR_TOOL   = Color(243, 98, 35);
    const auto COLOR_CHANCE = Color(115, 102, 189);
    const auto COLOR_INFO   = Color::White;



    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------

















    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------


    Average10000_RANDOM_GEN = Average10000_RANDOM_GEN_SOURCE;
    



    map<int, double> freq2;
    for (int val : Average10000_RANDOM_GEN) freq2[val]++;

    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------





    













    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------

    // --- Биннинг чтобы не потерять данные---
    int binSize = 1;

    int maxItV = *max_element(Average10000.begin(), Average10000.end());


    if (maxItV > 1000)
    {
        binSize = maxItV / 100;
    }


    // Частоты по корзинам для Average10000
    map<int, double> freqBinned;
    for (int val : Average10000)
    {
        int bin = val / binSize;
        freqBinned[bin]++;
    }


    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------



























    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------


    // Максимум частоты для масштабирования
    auto mode1 = max_element(freqBinned.begin(), freqBinned.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
    auto mode2 = max_element(freq2.begin(), freq2.end(), [](const auto& a, const auto& b) { return a.second < b.second; });




    int modeValue1 = mode1->first * binSize; // домножаем на binSize
    int maxFreq1 = static_cast<int>(mode1->second);
    double maxFreq2 = mode2->second;



    float yScale1 = graphHeight / log2(1.f + maxFreq1);
    float yScale2 = 300.f / static_cast<double>(maxFreq2);




    const double smoothWindow = 5.f;
    const double smoothWindow2 = 5.5f;

    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------










    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------



    // --- Сглаживание для биннинга freqBinned ---
    vector<pair<double, double>> smoothedFreq1;
    for (auto it = freqBinned.begin(); it != freqBinned.end(); ++it)
    {
        double sum = 0, count = 0;
        for (int offset = -static_cast<int>(smoothWindow); offset <= static_cast<int>(smoothWindow); ++offset)
        {
            auto neighbor = freqBinned.find(it->first + offset);
            if (neighbor != freqBinned.end())
            {
                sum += neighbor->second;
                count++;
            }
        }
        smoothedFreq1.emplace_back(it->first, sum / max(1.0, count));
    }





    // Сглаживание для freq2
    vector<pair<double, double>> smoothedFreq2;
    for (auto it = freq2.begin(); it != freq2.end(); ++it)
    {
        double sum = 0, count = 0;
        for (int offset = -static_cast<int>(smoothWindow2); offset <= static_cast<int>(smoothWindow2); ++offset)
        {
            auto neighbor = freq2.find(it->first + offset);
            if (neighbor != freq2.end())
            {
                sum += neighbor->second;
                count++;
            }
        }
        smoothedFreq2.emplace_back(it->first, sum / max(1.0, count));
    }

    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------



















    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------


    float xStart = 70.f;
    float xEnd = window.getSize().x - 30.f; // справа небольшой отступ


    float plotWidth = xEnd - xStart;




    // ---  --- ---------- ------------- ---------- ------------- ---------- ------------- ---------- ------------- ---------- ----------

    float xStep2 = plotWidth / max(1.f, static_cast<float>(smoothedFreq2.size()));

    // --- Отрисовка графика freq2 (синий) ---
    VertexArray curve2(PrimitiveType::LineStrip);
    float x2 = 70.f;
    for (const auto& [val, count] : smoothedFreq2)
    {
        float y = window.getSize().y - marginBottom - static_cast<float>(count) * yScale2;
        curve2.append(Vertex(Vector2f(x2, y), COLOR_CHANCE));
        x2 += xStep2;
    }
    window.draw(curve2);

    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------


    // Горизонтальные линии min/max freq2
    auto [minIt, maxIt] = minmax_element(smoothedFreq2.begin(), smoothedFreq2.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    float yMinVal = window.getSize().y - marginBottom - static_cast<float>(minIt->second) * yScale2;
    float yMaxVal = window.getSize().y - marginBottom - static_cast<float>(maxIt->second) * yScale2;

    VertexArray minLine(PrimitiveType::Lines, 2);
    minLine.append(Vertex(Vector2f(70.f, yMinVal), COLOR_CHANCE));
    minLine.append(Vertex(Vector2f(window.getSize().x - 30.f, yMinVal), COLOR_CHANCE));

    VertexArray maxLine(PrimitiveType::Lines, 2);
    maxLine.append(Vertex(Vector2f(70.f, yMaxVal), COLOR_CHANCE));
    maxLine.append(Vertex(Vector2f(window.getSize().x - 30.f, yMaxVal), COLOR_CHANCE));

    window.draw(minLine);
    window.draw(maxLine);

    int minVal = *min_element(Average10000_RANDOM_GEN.begin(), Average10000_RANDOM_GEN.end());
    int maxVal = *max_element(Average10000_RANDOM_GEN.begin(), Average10000_RANDOM_GEN.end());

    // Подписи слева для freq2
    Text minText(font);
    minText.setCharacterSize(13);
    minText.setFillColor(COLOR_CHANCE);
    minText.setString(L"min:" + to_wstring(minVal));
    minText.setPosition(Vector2f(10.f, yMinVal - 7.f));
    window.draw(minText);

    Text maxText(font);
    maxText.setCharacterSize(13);
    maxText.setFillColor(COLOR_CHANCE);
    maxText.setString(L"max: " + to_wstring(maxVal));
    maxText.setPosition(Vector2f(10.f, yMaxVal - 7.f));
    window.draw(maxText);

    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------

















    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------










    // Получаем отсортированные ключи корзин (binsKeys) для корректного распределения по оси X
    vector<int> binsKeys;
    for (const auto& [k, _] : freqBinned) binsKeys.push_back(k);


    std::sort(binsKeys.begin(), binsKeys.end());

    // Количество точек минус 1 для корректного деления 
    size_t binCount = binsKeys.size();


    float xStep1 = (binCount > 1) ? (plotWidth / (binCount - 1)) : 0.f;




    // Отрисовка красного графика с ограничением по горизонтали
    VertexArray curve1(PrimitiveType::LineStrip);

    for (size_t i = 0; i < binCount; ++i)
    {
        int bin = binsKeys[i];
        double count = 0;
        // Найти значение count для этого bin в smoothedFreq1
        auto it = std::find_if(smoothedFreq1.begin(), smoothedFreq1.end(),
            [bin](const auto& p) { return static_cast<int>(p.first) == bin; });
        if (it != smoothedFreq1.end())
            count = it->second;

        float y = window.getSize().y - marginBottom - std::log2(1 + count) * yScale1;
        float x = xStart + i * xStep1;

        // Гарантируем, что x не выходит за границы окна
        if (x < xStart) x = xStart;
        if (x > xEnd) x = xEnd;

        curve1.append(Vertex(Vector2f(x, y), COLOR_TOOL));
    }
    window.draw(curve1);


    const size_t numLabels = 15;
    if (binsKeys.size() >= 5)
    {
        float labelStep = plotWidth / (numLabels - 1);
        float labelX = 70.f;

        for (size_t i = 0; i < numLabels; ++i)
        {
            size_t idx = static_cast<size_t>((i * (binsKeys.size() - 1)) / (numLabels - 1));
            int binValue = binsKeys[idx];

            Text label(font);
            label.setCharacterSize(14);
            label.setFillColor(COLOR_INFO);
            // Надпись — диапазон корзины
            label.setString(to_string(binValue * binSize));
            label.setPosition(Vector2f(labelX - 10.f, window.getSize().y - marginBottom + 5.f));

            window.draw(label);
            labelX += labelStep;
        }
    }




    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------


















    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------


    // --- Часто встречающиеся значения ---
    float rangeThreshold = 0.6f;
    vector<int> frequentValues;
    for (const auto& [bin, count] : freqBinned)
    {
        if ((float)count >= (float)maxFreq1 * rangeThreshold)
        {
            frequentValues.push_back(bin * binSize);
        }
    }



    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------













    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------

    Text label(font);
    label.setCharacterSize(14);
    label.setFillColor(COLOR_INFO);


    // ----------- Ошибки -----------
    wstring error = L"ERROR -> ";

    int n = static_cast<int>(Average10000.back());

    switch (n)
    {
    case -3: error += L"одна или более характеристик оказалась, больше чем нужно -> log";
        break;
    case -2: error += L"попытка уменьшить мод, нельзя уменьшить мод вручную"; 
        break;
    case -4: error += L"не получилось повысить характеристику (MA = 200) -> log"; 
        break;
    case -9: error += L"???????"; 
        break;
    case -7: error += L"процент равен или меньше 0, невозможно повысить характеристику"; 
        break;
    case -23: error += L"не получилось повысить характеристику (MA = 200)";
        break;
    default: error = L""; break;
    }





    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------














    /// --- ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ------------ - ---------- ----------


    int RESULTCOUNTBOX = accumulate(Average10000.begin(), Average10000.end(), 0);

    int stepLabel = 15;

    label.setString(L"Среднее кол-во по всему диапазону: " + to_string(RESULTCOUNTBOX / static_cast<int>(Average10000.size())));
    label.setPosition(Vector2f(10, stepLabel * 0));
    window.draw(label);

    label.setString(L"мода графика (частовстречающееся кол-во инстр.для текущего мода): " + to_string(modeValue1));
    label.setPosition(Vector2f(10, stepLabel));
    window.draw(label);

    if (!frequentValues.empty())
    {
        int minFreqVal = *min_element(frequentValues.begin(), frequentValues.end());
        int maxFreqVal = *max_element(frequentValues.begin(), frequentValues.end());

        label.setFillColor(COLOR_INFO);
        label.setString(L"underprice меньше: " + to_wstring(minFreqVal));
        label.setPosition(Vector2f(10, stepLabel * 2));
        window.draw(label);

        label.setString(L"overprice больше: " + to_wstring(maxFreqVal));
        label.setPosition(Vector2f(10, stepLabel * 3));
        window.draw(label);
    }

    label.setString(error);
    label.setFillColor(Color::Magenta);
    label.setPosition(Vector2f(10, stepLabel * 4));
    window.draw(label);

    label.setString(L"колличество оружия, модифицированного до указанной модификации: " + to_string(totalIterations));
    label.setFillColor(COLOR_INFO);
    label.setPosition(Vector2f(10, stepLabel * 5));
    window.draw(label);

    label.setString(L"инструменты");
    label.setFillColor(COLOR_TOOL);
    label.setPosition(Vector2f(10, stepLabel * 6));
    window.draw(label);

    label.setFillColor(COLOR_CHANCE);
    label.setString(L"Сгенерированная попытка 0-100% |mt19937+random_device| колличество:" + to_string(Average10000_RANDOM_GEN.size()));
    label.setPosition(Vector2f(10, stepLabel * 7));
    window.draw(label);


    label.setFillColor(COLOR_TOOL);
    label.setString(L"90й перцентиль (идем по серии неудач, но не превысим) : " + to_string(percentile(Average10000, 0.90)));
    label.setPosition(Vector2f(10, stepLabel * 8));
    window.draw(label);

    label.setFillColor(COLOR_TOOL);
    label.setString(L"50й перцентиль (с шансом 50% понадобится столько): " + to_string(percentile(Average10000, 0.50)));
    label.setPosition(Vector2f(10, stepLabel * 9));
    window.draw(label);

    label.setString(L"Чаще");
    label.setPosition(Vector2f(10, stepLabel * 9 + 20));
    window.draw(label);

    label.setString(L"реже");
    label.setPosition(Vector2f(10, window.getSize().y - stepLabel - marginBottom));
    window.draw(label);

    label.setString(L"кол-во");
    label.setPosition(Vector2f(10, window.getSize().y - marginBottom + 5.f));
    window.draw(label);
















    

    label.setCharacterSize(16);
    label.setFillColor(COLOR_INFO);
    label.setString(L"оружие: " + NameGun);
    label.setPosition(Vector2f(static_cast<float>(window.getSize().x * 0.37), 0));
    window.draw(label);


    // по у
    int yh = 25;
    // коэфффициент по х
    float xl_k = 0.50;

    label.setString(L"начальный мод:");
    label.setPosition(Vector2f(static_cast<float>(window.getSize().x * xl_k), yh * 0));
    window.draw(label);
    for (int i = 0 ; i < INITIAL_CHARACTERISTIC.size(); i++)
    {
        auto procent = INITIAL_CHARACTERISTIC[i];
        if      (procent > 0.0) { label.setFillColor(Color::Green); }
        else if (procent < 0.0) { label.setFillColor(Color::Red);   }
        else                    { label.setFillColor(Color::White); }

        label.setString(format("{:.1f}", procent));
        label.setPosition(Vector2f(static_cast<float>(window.getSize().x * xl_k + (i * 40)) , yh * 1));
        window.draw(label);
    }



    label.setFillColor(COLOR_INFO);
    label.setString(L"указанный мод:");

    label.setPosition(Vector2f(static_cast<float>(window.getSize().x * xl_k), yh * 2));
    window.draw(label);
    for (int i = 0; i < START_CHARACTERISTIC.size(); i++)
    {
        auto procent = START_CHARACTERISTIC[i];
        if (procent > 0.0) { label.setFillColor(Color::Green); }
        else if (procent < 0.0) { label.setFillColor(Color::Red); }
        else { label.setFillColor(Color::White); }


        label.setString(format("{:.1f}", procent));
        label.setPosition(Vector2f(static_cast<float>((window.getSize().x * xl_k) + (i * 40)), yh * 3));
        window.draw(label);
    }

    label.setFillColor(COLOR_INFO);
    label.setString(L"результат, в ходе сборки методом:");
    label.setPosition(Vector2f(static_cast<float>(window.getSize().x * xl_k), yh * 4));
    window.draw(label);

    for (int i = 0; i < RESULT_CHARACTERISTIC.size(); i++)
    {

        auto procent = RESULT_CHARACTERISTIC[i];
        if      (procent > 0.0) { label.setFillColor(Color::Green); }
        else if (procent < 0.0) { label.setFillColor(Color::Red);   }
        else                    { label.setFillColor(Color::White); }

        label.setString(format("{:.1f}", procent));
        label.setPosition(Vector2f(static_cast<float>((window.getSize().x * xl_k) + (i * 40)), yh * 5));
        window.draw(label);
    }

    label.setFillColor(COLOR_INFO);















    xl_k = 0.73;


    map<int, wstring> statNameMap = {
        {0, L"Кучность"},               
        {1, L"Темп стрельбы"},                          
        {2, L"Отдача оружия"},           
        {3, L"Качание оружия"},  
        {4, L"Пробитие"},
        {5, L"Отказ из-за сост."},  
        {6, L"Отказ из-за грязи"}        
    };

    label.setString(L"метод: начало:");
    label.setCharacterSize(14);
    label.setPosition(Vector2f(static_cast<float>((window.getSize().x * xl_k + 45)), 0));
    window.draw(label);

    yh = 15;
    for (int i = 0; i < method.size(); i++)
    {
        label.setString(to_string(i+1) + L" - " + statNameMap[method[i]]);
        label.setPosition(Vector2f(static_cast<float>((window.getSize().x * xl_k + 45)), yh * (i+1)));
        window.draw(label);
    }
    
















    map<double, wstring> ToolModifierToStr = {
    { 0.0, L"Без инструмента" },
    { 0.2, L"Старый инструмент" },
    { 0.5, L"Обычный инструмент" },
    { 0.8, L"Улучшенный инструмент" }
    };

    map<double, wstring> KitModifierToStr = {
    { 0.0, L"Без набора" },
    { 1.0, L"Ремнабор красный" },
    { 4.0, L"запчасти серые" }
    };

    map<double, wstring> SkillModifierToStr = {
    { 0.0, L"Без навыка" },
    { 0.2, L"Игрок: Мастер 1 ур." },
    { 0.4, L"Игрок: Мастер 2 ур." },
    { 0.6, L"Игрок: Мастер 3 ур." },
    { 0.8, L"Игрок: Мастер 4 ур." },
    { 1.0, L"Игрок: Мастер 5 ур." },
    { 0.5, L"NPC: Мастер 2 ур." },
    { 1.5, L"NPC: Мастер 4 ур." },
    { 2.0, L"NPC: Мастер 5 ур." }
    };

    xl_k = 0.88;

    label.setString(L"модификаторы:");
    label.setPosition(Vector2f(static_cast<float>((window.getSize().x * xl_k)), 0));
    window.draw(label);

    
    label.setString(ToolModifierToStr[get<0>(Tool_Kit_Skil)]);
    label.setPosition(Vector2f(static_cast<float>((window.getSize().x * xl_k)), yh * 1));
    window.draw(label);

    label.setString(KitModifierToStr[get<1>(Tool_Kit_Skil)]);
    label.setPosition(Vector2f(static_cast<float>((window.getSize().x * xl_k)), yh * 2));
    window.draw(label);

    label.setString(SkillModifierToStr[get<2>(Tool_Kit_Skil)]);
    label.setPosition(Vector2f(static_cast<float>((window.getSize().x * xl_k)), yh * 3));
    window.draw(label);

    return { modeValue1 };
}

