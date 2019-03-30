#include "path.h"

namespace Ks
{
    std::string FormatFilePath(const std::string & _filepath)
	{
		int nSec = 0;
		std::string curSec;
		std::string fpath;
		if( _filepath[0] == '/') {
            fpath.push_back('/');
        }
		const char * ptr = _filepath.c_str();
		while (*ptr != 0)
		{
			if (*ptr == '\\' || *ptr == '/')
			{
				if (curSec.length() > 0)
				{
					if (curSec == ".") {}
					else if (curSec == ".." && nSec >= 2)
					{
						int secleft = 2;
						while (!(fpath.empty() && secleft == 0))
						{
							if (fpath.back() == '\\' || fpath.back() == '/')
							{
								--secleft;
								break;
							}
							fpath.pop_back();
						}
					}
					else
					{
						if( !fpath.empty() && fpath.back()!='/' )
							fpath.push_back('/');
						fpath.append(curSec);
						++nSec;
					}
					curSec.clear();
				}
			}
			else
			{
				curSec.push_back( *ptr );
				if (*ptr == ':')
				{
					--nSec;
				}
			}
			++ptr;
		}
		if (curSec.length() > 0)
		{
			if (!fpath.empty() && fpath.back() != '/')
				fpath.push_back('/');
			fpath.append(curSec);
		}
		return fpath;
	}
}