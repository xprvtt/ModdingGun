#pragma once

#include "Core.h"
#include "GunStats.h"
#include "ThreadAssemble.h"
#include <limits>
#include "GulText.h"




struct CurvePack
{
    RectangleShape MainRectangleCurve;

    vector< GUI_TextAndRectangle> VGUITextCurve;

    VertexArray    Curve;
};


/// <summary>
/// получаем кривую, отображающую частоту 
/// </summary>
/// <param name="RectangleOther"></param>
/// <param name="FreqModifiers"></param>
/// <param name="ColorCurve"></param>
/// <param name="name"></param>
/// <returns></returns>
CurvePack GetCurveFrequrency
(
    RectangleShape RectangleOther,
    const map<CountModifiers, float>& FreqModifiers,

    Color ColorCurve,
    string name

);


/// <summary>
/// получаем частоту экземпляров
/// </summary>
/// <param name="OtherVectorCountModifiers"></param>
/// <returns></returns>
map<CountModifiers, float> GetFreqModifiers(const vector<CountModifiers>& OtherVectorCountModifiers);




/// <summary>
/// получаем сглаженный график по частоте/// </summary>
/// <param name="OtherMap"></param>
/// <param name="smoothnessPercent"></param>
/// <param name="original_min"></param>
/// <param name="original_max"></param>
/// <returns></returns>
map<CountModifiers, float> SmoothFreqModifiers(const map<CountModifiers, float>& OtherMap, float smoothnessPercent);




/// <summary>
/// получаем значение которое не привысим с шансом 
/// </summary>
/// <param name="freqMap"></param>
/// <param name="p"> перцентиль 0.0 - 100.0</param>
/// <returns></returns>
CountModifiers GetPercentile(const map<CountModifiers, float>& freqMap, float percentile);



