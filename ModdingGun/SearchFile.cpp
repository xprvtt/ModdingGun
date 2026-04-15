#include "Core.h"

vector<path> SearchFile(path searchInPath, string extension)
{
	vector<path> result;
	try
	{
		for (const auto& entry : directory_iterator(searchInPath))
		{
			if (extension == "")
			{
				if (entry.is_directory())
				{
					result.push_back(entry.path());
				}
			}
			else
			{
				if (entry.is_regular_file() && entry.path().extension() == extension)
				{
					result.push_back(entry.path());
				}
			}
		}
	}
	catch ([[maybe_unused]]const filesystem_error& error)
	{
		OUTPUT_LOG("SearchFile -> Error");
		return vector<path>();
	}
	return result;
}