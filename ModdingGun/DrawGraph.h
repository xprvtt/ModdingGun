#pragma once

#include "Core.h"
#include "GunStats.h"
#include "ThreadAssemble.h"
#include <limits>
#include "GulText.h"

struct CurvePack
{
    RectangleShape m_mainRectangleCurve;
    vector<GUITextAndRectangle> m_vGUITextCurve;
    VertexArray    m_curve;
};


/// <summary>
/// получаем кривую, отображающую частоту 
/// </summary>
/// <param name="rectangleOther"></param>
/// <param name="freqModifiers"></param>
/// <param name="colorCurve"></param>
/// <param name="name"></param>
/// <returns></returns>
CurvePack GetCurveFrequrency
(
    RectangleShape rectangleOther,
    const map<CountModifiers, float>& freqModifiers,
    Color colorCurve,
    string name

);

VertexArray CurveFun(RectangleShape mainRectangleCurve, Color colorCurve, const map<CountModifiers, float>& freqModifiers);

/// <summary>
/// получаем частоту экземпляров
/// </summary>
/// <param name="otherVectorCountModifiers"></param>
/// <returns></returns>
map<CountModifiers, float> GetFreqModifiers(const vector<CountModifiers>& otherVectorCountModifiers);

map<CountModifiers,float> GetBinedFreqModifiers(const map<CountModifiers, float>& otherMap, float percentBin);

/// <summary>
/// получаем сглаженный график по частоте/// </summary>
/// <param name="otherMap"></param>
/// <param name="smoothnessPercent"></param>
/// <param name="original_min"></param>
/// <param name="original_max"></param>
/// <returns></returns>
map<CountModifiers, float> GetSmoothFreqModifiers(const map<CountModifiers, float>& otherMap, float smoothnessPercent);

/// <summary>
/// получаем значение которое не привысим с шансом 
/// </summary>
/// <param name="freqMap"></param>
/// <param name="p"> перцентиль 0.0 - 100.0</param>
/// <returns></returns>
CountModifiers GetPercentile(const map<CountModifiers, float>& freqMap, float percentile);
