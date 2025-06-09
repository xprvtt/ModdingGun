#include "Core.h"

vector<path> SearchFile(path SearchInPath, string Extension)
{
	vector<path> result;
	try
	{
		for (const auto& entry : directory_iterator(SearchInPath))
		{
			if (Extension == "")
			{
				if (entry.is_directory())
				{
					result.push_back(entry.path());
				}
			}
			else
			{
				if (entry.is_regular_file() && entry.path().extension() == Extension)
				{
					result.push_back(entry.path());
				}
			}
		}
	}
	catch (const filesystem_error& error)
	{
		OutputLog("SearchFile -> Error");
		return vector<path>();
	}
	return result;
}