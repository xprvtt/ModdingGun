#include "DrawGraph.h"
#include "LangManager.h"
#include "ThreadAssemble.h"



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

CurvePack GetCurveFrequrency
(
    RectangleShape MainRectangleCurve,
    const map<CountModifiers, float>& FreqModifiers,

    Color ColorCurve,
    string name
)
{
    ////////////////////////////////////////////////////////////////////////////////
    float kx = 0.2f;
    float ky = 0.1f;

    float XposGeneral = MainRectangleCurve.getPosition().x;
    float YposGeneral = MainRectangleCurve.getPosition().y;


    float XsizeRecText = MainRectangleCurve.getSize().x * kx;
    float YsizeRecText = MainRectangleCurve.getSize().y * ky;

    float Y_PrecenSizeCell = 0.9f;

    int CharacterSize = 14 * Y_PrecenSizeCell;
    int YpositionInfoText = 0;
    ////////////////////////////////////////////////////////////////////////////////






    ////////////////////////////////////////////////////////////////////////////////

    RectangleShape r0_Info_LeftTop = MainRectangleCurve;
    r0_Info_LeftTop.setPosition(Vector2f(XposGeneral - XsizeRecText, YposGeneral- YsizeRecText * (1 + YpositionInfoText)));
    r0_Info_LeftTop.setSize(Vector2f(XsizeRecText + MainRectangleCurve.getSize().x, YsizeRecText * Y_PrecenSizeCell));

    RectangleShape r1_LeftTop = MainRectangleCurve;
    r1_LeftTop.setPosition(Vector2f(XposGeneral - XsizeRecText, MainRectangleCurve.getPosition().y));
    r1_LeftTop.setSize(Vector2f(XsizeRecText, YsizeRecText * Y_PrecenSizeCell));

    RectangleShape r1_LeftDown = MainRectangleCurve;
    r1_LeftDown.setPosition(Vector2f(XposGeneral - XsizeRecText, YposGeneral+ MainRectangleCurve.getSize().y - YsizeRecText));
    r1_LeftDown.setSize(Vector2f(XsizeRecText, YsizeRecText * Y_PrecenSizeCell));

    RectangleShape r2_LeftDown = MainRectangleCurve;
    r2_LeftDown.setPosition(Vector2f(XposGeneral - XsizeRecText, YposGeneral+ MainRectangleCurve.getSize().y));
    r2_LeftDown.setSize(Vector2f(XsizeRecText, YsizeRecText * Y_PrecenSizeCell));

    RectangleShape r2_RightDown = MainRectangleCurve; 
    r2_RightDown.setPosition(Vector2f(XposGeneral + MainRectangleCurve.getSize().x, YposGeneral+ MainRectangleCurve.getSize().y));
    r2_RightDown.setSize(Vector2f(XsizeRecText, YsizeRecText * Y_PrecenSizeCell));


    vector< GUI_TextAndRectangle> VGUITextCurve =
    {
        {{name}, r0_Info_LeftTop ,false, false},
        {{"Often"}, r1_LeftTop      ,false, false},
        {{"Rarely"}, r1_LeftDown     ,false, false},
        {{"Count"}, r2_LeftDown     ,false, false},
        {{"CountMax"}, r2_RightDown    ,false, false},
    };
    VGUITextCurve[0].setColorText(ColorCurve);

    ////////////////////////////////////////////////////////////////////////////////








    //////////////////////////////////////////////////////////////////////////////////








    //////////////////////////////////////////////////////////////////////////////////

    if (FreqModifiers.empty())
    {
        OutputLog("GetCurveForModifiers -> FreqModifiers empty");
        VGUITextCurve[0].setKeyString_and_TextWstring({ "Empty" });

        return { MainRectangleCurve, VGUITextCurve, VertexArray() };
    }
    //////////////////////////////////////////////////////////////////////////////////
















    //////////////////////////////////////////////////////////////////////////////////
    float Xsize = MainRectangleCurve.getSize().x;
    float Ysize = MainRectangleCurve.getSize().y;
    float Xpos  = XposGeneral;
    float Ypos  = MainRectangleCurve.getPosition().y;

    VertexArray curve(PrimitiveType::LineStrip, FreqModifiers.size());
    //////////////////////////////////////////////////////////////////////////////////




    //////////////////////////////////////////////////////////////////////////////////
    // 1. Найдём максимальную частоту
    float min_freq = FLT_MAX;
    float max_freq = -FLT_MAX;

    for (const auto& [_, freq] : FreqModifiers)
    {
        min_freq = min(min_freq, freq);
        max_freq = max(max_freq, freq);
    }
    //////////////////////////////////////////////////////////////////////////////////



    //////////////////////////////////////////////////////////////////////////////////
    // 2. Генерируем график
    int i = 0;
    float dx = Xsize / max(1.0f, static_cast<float>(FreqModifiers.size() - 1));

    for (auto& [_, freq_count] : FreqModifiers)
    {
        float x = Xpos + dx * i;

        // Нормализуем: 1 -> нижняя граница, max_freq -> верхняя граница
        float range = max(0.0001f, max_freq - min_freq); // защита от деления на 0
        float t = (freq_count - min_freq) / range;
        float y = Ypos + Ysize * (1.f - t);

        curve[i].position = Vector2f(x, y);
        curve[i].color = ColorCurve;

        i++;
    }
    //////////////////////////////////////////////////////////////////////////////////




    return { MainRectangleCurve, VGUITextCurve,  curve };
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////















//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


map<CountModifiers, float> GetFreqModifiers(const vector<CountModifiers>& OtherVectorCountModifiers)
{
    map<CountModifiers, float> result;

    if (OtherVectorCountModifiers.empty())
    {
        OutputLog("GetFreqCountModifiers -> empty");
    }

    for (size_t i = 0; i < OtherVectorCountModifiers.size(); i++)
    {
        result[OtherVectorCountModifiers[i]]++;
    }
    return result;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

















//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


map<CountModifiers, float> SmoothFreqModifiers(const map<CountModifiers, float>& OtherMap, float smoothnessPercent)
{

    //////////////////////////////////////////////////////////////////////////////////
    map<CountModifiers, float> result;

    if (OtherMap.empty())
    {
        OutputLog("GetFreqCountModifiers -> empty");
        return result;
    }
    //////////////////////////////////////////////////////////////////////////////////









    //////////////////////////////////////////////////////////////////////////////////
    int totalSize = static_cast<int>(OtherMap.size());
    float radius = max(0, smoothnessPercent * totalSize);

    vector<pair<CountModifiers, float>> items(OtherMap.begin(), OtherMap.end());

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
    //////////////////////////////////////////////////////////////////////////////////




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



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////















