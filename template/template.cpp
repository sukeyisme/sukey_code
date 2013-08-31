#include <stdio.h>
#include <string>

template <typename T>

inline T const& max(T const&a,T const&b)
{
	return a<b?b:a;
}

template <typename T>
inline T* const& max(T* const&a,T* const&b)
{
	return a<b?b:a;
}

inline char const* const& max(char const* const&a,char const* const&b)
{
	return std::strcmp(a,b)<0?b:a;
}
int main()
{
	

	return 0;
}



