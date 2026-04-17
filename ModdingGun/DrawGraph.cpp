#include "DrawGraph.h"
#include "LangManager.h"
#include "ThreadAssemble.h"


//-----------------------------------------------------------------------------------------------------------------------

CurvePack getCurveFrequrency(RectangleShape mainRectangleCurve, const map<CountModifiers, float>& frequerencyModifiers, Color colorCurve, string name)
{
	const float kx = 0.2f;
	const float ky = 0.1f;

	const float xPosGeneral = mainRectangleCurve.getPosition().x;
	const float yPosGeneral = mainRectangleCurve.getPosition().y;

	const float xSizeRecText = mainRectangleCurve.getSize().x * kx;
	const float ySizeRecText = mainRectangleCurve.getSize().y * ky;

	const float yPrecenSizeCell = 0.9f;

	const int characterSize = static_cast<int>(14 * yPrecenSizeCell);
	const int yPositionInfoText = 0;

	RectangleShape r0InfoLeftTop = mainRectangleCurve;
	r0InfoLeftTop.setPosition(Vector2f(xPosGeneral - xSizeRecText, yPosGeneral - ySizeRecText * (1 + yPositionInfoText)));
	r0InfoLeftTop.setSize(Vector2f(xSizeRecText + mainRectangleCurve.getSize().x, ySizeRecText * yPrecenSizeCell));

	RectangleShape r1LeftTop = mainRectangleCurve;
	r1LeftTop.setPosition(Vector2f(xPosGeneral - xSizeRecText, mainRectangleCurve.getPosition().y));
	r1LeftTop.setSize(Vector2f(xSizeRecText, ySizeRecText * yPrecenSizeCell));

	RectangleShape r1LeftDown = mainRectangleCurve;
	r1LeftDown.setPosition(Vector2f(xPosGeneral - xSizeRecText, yPosGeneral + mainRectangleCurve.getSize().y - ySizeRecText));
	r1LeftDown.setSize(Vector2f(xSizeRecText, ySizeRecText * yPrecenSizeCell));

	RectangleShape r2LeftDown = mainRectangleCurve;
	r2LeftDown.setPosition(Vector2f(xPosGeneral - xSizeRecText, yPosGeneral + mainRectangleCurve.getSize().y));
	r2LeftDown.setSize(Vector2f(xSizeRecText, ySizeRecText * yPrecenSizeCell));

	RectangleShape r2RightDown = mainRectangleCurve;
	r2RightDown.setPosition(Vector2f(xPosGeneral + mainRectangleCurve.getSize().x, yPosGeneral + mainRectangleCurve.getSize().y));
	r2RightDown.setSize(Vector2f(xSizeRecText, ySizeRecText * yPrecenSizeCell));

	vector< GUITextAndRectangle> m_arrayGUITextCurve =
	{
		{{name}, r0InfoLeftTop ,false, false},
		{{"Often"}, r1LeftTop      ,false, false},
		{{"Rarely"}, r1LeftDown     ,false, false},
		{{"CountMin"}, r2LeftDown     ,false, false},
		{{"CountMax"}, r2RightDown    ,false, false},
	};
	m_arrayGUITextCurve[0].setColorText(colorCurve);

	if (frequerencyModifiers.empty())
	{
		OUTPUT_LOG("GetCurveForModifiers -> frequerencyModifiers empty");
		m_arrayGUITextCurve[0].setKeyStringTextWstring({ "Empty" });

		return { mainRectangleCurve, m_arrayGUITextCurve, VertexArray() };
	}
	return { mainRectangleCurve, m_arrayGUITextCurve,  curveFun(mainRectangleCurve, colorCurve, frequerencyModifiers) };
}

//-----------------------------------------------------------------------------------------------------------------------

VertexArray curveFun(RectangleShape mainRectangleCurve, Color colorCurve, const map<CountModifiers, float>& freqModifiers)
{
	const float xSize = mainRectangleCurve.getSize().x;
	const float ySize = mainRectangleCurve.getSize().y;
	const float xPos = mainRectangleCurve.getPosition().x;;
	const float yPos = mainRectangleCurve.getPosition().y;

	VertexArray curve(PrimitiveType::LineStrip, freqModifiers.size());

	// 1. Найдём максимальную частоту
	float minFreq = FLT_MAX;
	float maxFreq = -FLT_MAX;

	for (const auto& [_, currentFreq] : freqModifiers)
	{
		minFreq = min(minFreq, currentFreq);
		maxFreq = max(maxFreq, currentFreq);
	}

	// 2. Генерируем график
	int i = 0;
	float dx = xSize / max(1.0f, static_cast<float>(freqModifiers.size() - 1));
	for (auto& [_, countFreq] : freqModifiers)
	{
		float x = xPos + dx * i;

		// Нормализуем: 1 -> нижняя граница, max_freq -> верхняя граница
		float range = max(0.0001f, maxFreq - minFreq); // защита от деления на 0
		float t = (countFreq - minFreq) / range;
		float y = yPos + ySize * (1.f - t);

		curve[i].position = Vector2f(x, y);
		curve[i].color = colorCurve;

		i++;
	}
	return curve;
}

//-----------------------------------------------------------------------------------------------------------------------

map<CountModifiers, float> getFreqModifiers(const vector<CountModifiers>& otherVectorCountModifiers)
{
	if (otherVectorCountModifiers.empty())
	{
		OUTPUT_LOG("GetFreqCountModifiers -> empty");
		return map<CountModifiers, float>();
	}

	std::map<CountModifiers, float> result;
	for (const auto& mod : otherVectorCountModifiers)
	{
		result[mod]++;
	}
	return result;
}

//-----------------------------------------------------------------------------------------------------------------------

map<CountModifiers, float> getBinedFreqModifiers(const map<CountModifiers, float>& otherMap, float percentBin)
{
	if (otherMap.empty()) { return {}; }

	// Если указано 100%, логично биннинг не нужен
	if (percentBin >= 1.0f) { return otherMap; }

	// Определение количества корзин
	int сountBascet = max(1, static_cast<int>(otherMap.size() * percentBin));

	// Сбор и сортировка всех частот с привязкой к CountModifiers

	vector<pair<CountModifiers, float>> sortedFreqs(otherMap.begin(), otherMap.end());

	sort(sortedFreqs.begin(), sortedFreqs.end(),
		[](const auto& a, const auto& b)
		{
			return a.second < b.second;
		});

	// Распределение по корзинам по квантилям
	map<int, vector<pair<CountModifiers, float>>> bins;

	int itemsPerBin = static_cast<int>(sortedFreqs.size()) / сountBascet;

	int currentBin = 0;
	int countInCurrent = 0;

	for (const auto& [mod, freq] : sortedFreqs)
	{
		bins[currentBin].push_back({ mod, freq });
		countInCurrent++;

		if (countInCurrent >= itemsPerBin && currentBin + 1 < сountBascet)
		{
			currentBin++;
			countInCurrent = 0;
		}
	}

	// Формирование результата на основе среднего значения в корзине
	map<CountModifiers, float> result;
	for (const auto& [binIdx, items] : bins)
	{
		if (items.empty()) { continue; }

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

//-----------------------------------------------------------------------------------------------------------------------

map<CountModifiers, float> getSmoothFreqModifiers(const map<CountModifiers, float>& otherMap, float smoothnessPercent)
{

	if (otherMap.empty())
	{
		OUTPUT_LOG("GetFreqCountModifiers -> empty");
		return map<CountModifiers, float>();
	}

	if (smoothnessPercent == 0)
	{
		return otherMap;
	}

	int totalSize = static_cast<int>(otherMap.size());
	float radius = max(0, smoothnessPercent * totalSize);

	vector<pair<CountModifiers, float>> items(otherMap.begin(), otherMap.end());

	map<CountModifiers, float> result;
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

//-----------------------------------------------------------------------------------------------------------------------

CountModifiers getPercentile(const map<CountModifiers, float>& freqMap, float percentile)
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

//-----------------------------------------------------------------------------------------------------------------------