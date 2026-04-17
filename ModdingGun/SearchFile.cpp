#include "Core.h"

vector<path> searchFile(path searchInPath, string extension)
{
	try
	{
		vector<path> result;

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

		return result;
	}
	catch ([[maybe_unused]] const filesystem_error& error)
	{
		OUTPUT_LOG("SearchFile -> Error");
		return vector<path>();
	}
}